/* $Id: assemble.c,v 1.10 2023/09/17 20:47:27 leavens Exp $ */
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "utilities.h"
#include "assemble.h"
#include "bof.h"
#include "symtab.h"
#include "id_attrs.h"
#include "regname.h"

// Return the address associated with the lora l
static address_type assemble_lora2address(lora_t l)
{
    address_type ret = l.addr;
    if (!(l.address_defined)) {
	id_attrs *ida = symtab_lookup(l.label);
	if (ida == NULL) {
	    bail_with_error("Label \"%s\" was never defined!", l.label);
	}
	ret = ida->addr;
    }
    return ret;
}

// Assemble the code for prog, with output going to bf
void assembleProgram(BOFFILE bf, program_t prog)
{
    BOFHeader bh;
    strcpy(bh.magic, "BOF");
    bh.text_start_address = assemble_lora2address(prog.textSection.entryPoint);
    bh.text_length = BYTES_PER_WORD
	* ast_list_length(prog.textSection.instrs.instrs);
    bh.data_start_address = prog.dataSection.static_start_addr;
    bh.data_length = BYTES_PER_WORD
	* ast_list_length(prog.dataSection.staticDecls.decls);
    bh.stack_bottom_addr = prog.stackSection.stack_bottom_addr;
    bof_write_header(bf, bh);
    assembleTextSection(bf, prog.textSection);
    assembleDataSection(bf, prog.dataSection);
    // nothing to do for the stack section, it's all in the header
}

// Assemble the code for the given AST, with output going to bf
void assembleTextSection(BOFFILE bf, text_section_t ts)
{
    assembleAsmInstrs(bf, ts.instrs);
}

// Assemble the code for the given AST, with output going to bf
void assembleAsmInstrs(BOFFILE bf, asm_instrs_t instrs)
{
    asm_instr_t *ip = instrs.instrs;
    while (ip != NULL) {
	assembleAsmInstr(bf, *ip);
	ip = ip->next;
    }
}

// Assemble the code for the given AST, with output going to bf
void assembleAsmInstr(BOFFILE bf, asm_instr_t instr)
{
    assembleInstr(bf, instr.instr);
}

// Return the value of the immedidate data AST immed
static int immedData_value(immedData_t immed)
{
    int ret = 0;
    switch (immed.id_data_kind) {
    case id_number:
	ret = immed.data.immed;
	break;
    case id_syscall_code:
	ret = immed.data.syscall_code;
	break;
    case id_unsigned:
	ret = immed.data.uimmed;
	break;
    case id_lora:
        if (immed.data.lora.address_defined) {
	    return immed.data.lora.addr;
	} else {
	    id_attrs *idap = symtab_lookup(immed.data.lora.label);
	    if (idap == NULL) {
		bail_with_error("Label \"%s\" never defined!",
				immed.data.lora.label);
	    } else {
		return idap->addr;
	    }
	}
	break;
    case id_empty:
	ret = 0;
	break;
    default:
	bail_with_error("Unknown immed_data_kind_t (%d) in assemble_immedData_value",
			immed.id_data_kind);
	break;
    }
    return ret;
}

// Assemble the code for the given AST, with output going to bf
void assembleInstr(BOFFILE bf, instr_t instr)
{
    switch (instr.itype) {
    case syscall_instr_type:
	syscall_instr_t si;
	assert(instr.immed_kind == ik_syscall_code);
	si.op = REG_O;
	si.code = immedData_value(instr.immed_data);
	si.func = SYSCALL_F;
	instruction_write_syscallInstr(bf, si);
	break;
    case reg_instr_type:
	reg_instr_t ri;
	ri.op = REG_O;
	ri.func = instr.func;
	ri.rs = instr.regs[0];
	ri.rt = instr.regs[1];
	ri.rd = instr.regs[2];
	if (ri.func == SLL_F || ri.func == SRL_F) {
	    ri.shift = immedData_value(instr.immed_data);
	} else {
	    ri.shift = 0;
	}
	instruction_write_regInstr(bf, ri);
	break;
    case immed_instr_type:
	immed_instr_t ii;
	ii.op = instr.opcode;
	ii.rs = instr.regs[0];
	ii.rt = instr.regs[1];
	ii.immed = immedData_value(instr.immed_data);
	instruction_write_immedInstr(bf, instr.opcode, ii);
	break;
    case jump_instr_type:
	jump_instr_t ji;
	ji.op = instr.opcode;
	ji.addr = immedData_value(instr.immed_data);
	instruction_write_jumpInstr(bf, instr.opcode, ji);
	break;
    default:
	bail_with_error("Bad instr_type in assembleInstr (%d)!", instr.itype);
	break;
    }
}

// Assemble the code for the given AST, with output going to bf
void assembleDataSection(BOFFILE bf, data_section_t ds)
{
    assembleStaticDecls(bf, ds.staticDecls);
}

// Assemble the code for the given AST, with output going to bf
void assembleStaticDecls(BOFFILE bf, static_decls_t sds)
{
    static_decl_t *dcl = sds.decls;
    while (dcl != NULL) {
	assembleStaticDecl(bf, *dcl);
	dcl = dcl->next;
    }
}

// Assemble the code for the given AST, with output going to bf
void assembleStaticDecl(BOFFILE bf, static_decl_t dcl)
{
    bof_write_word(bf, dcl.initializer.number);
}
