/* $Id: asm_unparser.c,v 1.12 2023/09/17 21:43:41 leavens Exp $ */
#include <stdio.h>
#include "ast.h"
#include "utilities.h"
#include "asm_unparser.h"
#include "regname.h"

// Note: newlines and indentation are only printed
// by a function which deals with a grammar that sensibly needs one printed,
// as this makes them those functions more reusable.

// Unparse prog, with output going to the file out
void unparseProgram(FILE *out, program_t prog)
{
    unparseTextSection(out, prog.textSection);
    unparseDataSection(out, prog.dataSection);
    unparseStackSection(out, prog.stackSection);
    fprintf(out, ".end");
    newline(out);
}

// Unparse the given AST, with output going to out
void unparseTextSection(FILE *out, text_section_t ts)
{
    fprintf(out, ".text ");
    unparseEntryPoint(out, ts.entryPoint);
    newline(out);
    unparseAsmInstrs(out, ts.instrs);
}

// Unparse the given AST, with output going to out
void unparseEntryPoint(FILE *out, lora_t ep)
{
    unparseLora(out, ep);
}

// Unparse the given AST, with output going to out
void unparseLora(FILE *out, lora_t l)
{
    if (l.label != NULL) {
	fprintf(out, "%s", l.label);
    } else {
	fprintf(out, "%u", l.addr);
    }
}

// Unparse the given AST, with output going to out
void unparseAsmInstrs(FILE *out, asm_instrs_t instrs)
{
    asm_instr_t *ip = instrs.instrs;
    while (ip != NULL) {
	unparseAsmInstr(out, *ip);
	ip = ip->next;
    }
}

// Unparse the given AST, with output going to out
void unparseAsmInstr(FILE *out, asm_instr_t instr)
{
    unparseLabelOpt(out, instr.label_opt);
    unparseInstr(out, instr.instr);
    newline(out);
}

// Unparse the given AST, with output going to out
void unparseLabelOpt(FILE *out, label_opt_t lopt)
{
    if (lopt.name == NULL) {
	fprintf(out, "\t");
    } else {
	fprintf(out, "%s:\t", lopt.name);
    }
}

// Unparse the given AST, with output going to out
void unparseInstr(FILE *out, instr_t instr)
{
    fprintf(out, "%s ", instr.opname);

    // print any registers used
    int i = 0;
    if (instr.opcode == REG_O) {
	switch (instr.func) {
	case MFHI_F: case MFLO_F:
	    // these use only the third (rd) register	    
	    i = 2;
	    break;
	case SLL_F: case SRL_F:
	    // these use only the second (rt) and third (rd) registers
	    i = 1;
	    break;
	default:
	    // all the rest start with the rs register
	    i = 0;
	    break;
        }
    }
    for (/* initialized above */; i < instr.regs_used; i++) {
	unparseReg(out, instr.regs[i]);
	if (i != instr.regs_used-1) {
	    // only add a separating comma when there are more registers
	    fprintf(out, ", ");
	}
    }
    // Need a comma before an immediate argument?
    if (instr.regs_used > 0 && instr.immed_kind != ik_none) {
	fprintf(out, ", ");
    }
    // print any immediate arguments
    switch (instr.immed_kind) {
    case ik_immed:
	fprintf(out, "%hd", instr.immed_data.data.immed);
	break;
    case ik_uimmed:
	fprintf(out, "0x%hx", instr.immed_data.data.uimmed);
	break;
    case ik_syscall_code:
	// no arguments for system calls!
	break;
    case ik_offset:
	unparseLora(out, instr.immed_data.data.lora);
	break;
    case ik_none:
	/* do nothing in this case */
	break;
    default:
	bail_with_error("Unknown immed_kind_t in unparseInstr (%d)",
			instr.immed_kind);
	break;
    }
}

// Unparse a register, with output going to out
void unparseReg(FILE *out, unsigned short n)
{
    if (0 <= n && n < 32) {
	fprintf(out, "%s", regname_get(n));
    } else {
	bail_with_error("Bad register number (%d)!", n);
    }
}

// Unparse the given AST, with output going to out
void unparseDataSection(FILE *out, data_section_t ds)
{
    fprintf(out, ".data %u", ds.static_start_addr);
    newline(out);
    unparseStaticDecls(out, ds.staticDecls);
}

// Unparse the given AST, with output going to out
void unparseStaticDecls(FILE *out, static_decls_t sds)
{
    static_decl_t *dcl = sds.decls;
    while (dcl != NULL) {
	unparseStaticDecl(out, *dcl);
	dcl = dcl->next;
    }
}

// Unparse the given AST, with output going to out
void unparseStaticDecl(FILE *out, static_decl_t dcl)
{
    fprintf(out, "%s ", dcl.size_name);
    unparseIdent(out, dcl.ident);
    fprintf(out, " ");
    unparseInitializer(out, dcl.initializer);
    newline(out);
}

// Unparse the given AST, with output going to out
void unparseIdent(FILE *out, ident_t id)
{
    fprintf(out, "%s", id.name);
}

// Unparse the given AST, with output going to out
void unparseInitializer(FILE *out, initializer_t init)
{
    fprintf(out, "= %d", init.number);
}

// Unparse the given AST, with output going to out
void unparseStackSection(FILE *out, stack_section_t ss)
{
    fprintf(out, ".stack %u", ss.stack_bottom_addr);
    newline(out);
}
