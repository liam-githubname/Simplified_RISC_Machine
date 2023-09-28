// $Id: instruction.c,v 1.19 2023/09/18 17:17:55 leavens Exp $
#include <errno.h>
#include <string.h>
#include "bof.h"
#include "instruction.h"
#include "regname.h"
#include "utilities.h"
#include "machine_types.h"

#define INSTR_BUF_SIZE 512

// this declaration doesn't come in with <string.h>...
extern char *strdup(const char *s);

// space to hold one instruction's assembly language form
static char instr_buf[INSTR_BUF_SIZE];

// Return the type of the instruction given
instr_type instruction_type(bin_instr_t i) {
    switch (i.reg.op) { // need to pretend a type to read the op field
    case REG_O:
	if (i.reg.func == SYSCALL_F) {
	    return syscall_instr_type;
	} else {
	    return reg_instr_type;
	}
	break;
    case JMP_O:
    case JAL_O:
	return jump_instr_type;
	break;
    case ADDI_O:
    case ANDI_O:
    case BORI_O:
    case XORI_O:
    case BEQ_O:
    case BGEZ_O:
    case BGTZ_O:
    case BLEZ_O:
    case BLTZ_O:
    case BNE_O:
    case LBU_O:
    case LW_O:
    case SB_O:
    case SW_O:
	return immed_instr_type;
    default:
	return error_instr_type;
	break;
    }
}

// Requires: bof is open for reading in binary
// Read a single instruction (in binary) from bf and return it,
// but exit with an error if there is a problem.
bin_instr_t instruction_read(BOFFILE bf)
{
    bin_instr_t bi;
    size_t rd = fread(&bi, sizeof(bi), 1, bf.fileptr);
    if (rd != 1) {
	bail_with_error("Cannot read instruction from %s", bf.filename);
    }
    return bi;
}

void instr_write_bin_instr(BOFFILE bf, bin_instr_t i)
{
    size_t wr = fwrite(&i, sizeof(i), 1, bf.fileptr);
    if (wr != 1) {
	bail_with_error("Cannot write binary instr to %s", bf.filename);
    }
}

// Requires: bof is open for writing in binary
// Write the register instruction ri to bf in binary,
// but exit with an error if there is a problem.
void instruction_write_regInstr(BOFFILE bf, reg_instr_t ri)
{
    bin_instr_t bi;
    bi.reg = ri;
    bi.reg.op = REG_O;
    assert(instruction_type(bi) == reg_instr_type);
    instr_write_bin_instr(bf, bi);
}

// Requires: bof is open for writing in binary
// Write the system instruction si to bf in binary,
// but exit with an error if there is a problem.
void instruction_write_syscallInstr(BOFFILE bf, syscall_instr_t si)
{
    bin_instr_t bi;
    bi.syscall = si;
    bi.syscall.op = REG_O;
    bi.syscall.func = SYSCALL_F;
    assert(instruction_type(bi) == syscall_instr_type);
    instr_write_bin_instr(bf, bi);
}


// Requires: bof is open for writing in binary
// Write the immediate instruction ii with opcode op to bf in binary,
// but exit with an error if there is a problem.
void instruction_write_immedInstr(BOFFILE bf, unsigned short op,
				  immed_instr_t ii)
{
    bin_instr_t bi;
    bi.immed = ii;
    bi.immed.op = op;
    assert(instruction_type(bi) == immed_instr_type);
    instr_write_bin_instr(bf, bi);
}

// Requires: bof is open for writing in binary
// Write the jump instruction ji with opcode op to bf in binary,
// but exit with an error if there is a problem.
void instruction_write_jumpInstr(BOFFILE bf, unsigned short op,
				  jump_instr_t ji)
{
    bin_instr_t bi;
    bi.jump = ji;
    bi.jump.op = op;
    assert(instruction_type(bi) == jump_instr_type);
    instr_write_bin_instr(bf, bi);
}

// Requires: instr is a SYSCALL instruction
// (i.e., instr.op == 0 and instr.data.reg.func == SYSCALL_F).
// Return the code field that tells what kind of system call is being made
syscall_type instruction_syscall_number(bin_instr_t instr) {
    assert(instr.syscall.op == 0 && instr.syscall.func == SYSCALL_F);
    return instr.syscall.code;
}

// Return the mnemonic for the given system call code
const char *instruction_syscall_mnemonic(unsigned int code)
{
    switch (code) {
    case exit_sc:
	return "EXIT";
	break;
    case print_str_sc:
	return "PSTR";
	break;
    case print_char_sc:
	return "PCH";
	break;
    case read_char_sc:
	return "RCH";
	break;
    case start_tracing_sc:
	return "STRA";
	break;
    case stop_tracing_sc:
	return "NOTR";
	break;
    default:
	bail_with_error("Unknown code (%d) in instruction_syscall_mnemonic",
			code);
	return "NEVERHAPPENS";
	break;
    }
}

// Given a binary instruction, bi, for a register format instruction
// return a string giving the assembly language mnemonic for it
const char *instruction_func2name(bin_instr_t bi) {
    assert(bi.reg.op == REG_O);
    switch (bi.reg.func) {
    case ADD_F:
	return "ADD";
	break;
    case SUB_F:
	return "SUB";
	break;
    case MUL_F:
	return "MUL";
	break;
    case DIV_F:
	return "DIV";
	break;
    case MFHI_F:
	return "MFHI";
	break;
    case MFLO_F:
	return "MFLO";
	break;
    case AND_F:
	return "AND";
	break;
    case BOR_F:
	return "BOR";
	break;
    case NOR_F:
	return "NOR";
	break;
    case XOR_F:
	return "XOR";
	break;
    case SLL_F:
	return "SLL";
	break;
    case SRL_F:
	return "SRL";
	break;
    case JR_F:
	return "JR";
	break;
    case SYSCALL_F:
	return instruction_syscall_mnemonic(instruction_syscall_number(bi));
	break;
    default:
	bail_with_error("Unknown function code (%d) in instruction_func2name",
			bi.reg.func);
	break;
    }
    return NULL;
}

// Return the assembly language name (mnemonic) for i
const char *instruction_mnemonic(bin_instr_t i) {
    switch (i.immed.op) { // pretend it's an immediate instruction
    case REG_O:
	return instruction_func2name(i);	    
	break;
    case ADDI_O:
	return "ADDI";
	break;
    case ANDI_O:
	return "ANDI";
	break;
    case BORI_O:
	return "BOI";
	break;
    case XORI_O:
	return "XORI";
	break;
    case BEQ_O:
	return "BEQ";
	break;
    case BGEZ_O:
	return "BGEZ";
	break;
    case BGTZ_O:
	return "BGTZ";
	break;
    case BLEZ_O:
	return "BLEZ";
	break;
    case BLTZ_O:
	return "BLTZ";
	break;
    case BNE_O:
	return "BNE";
	break;
    case LBU_O:
	return "LBU";
	break;
    case LW_O:
	return "LW";
	break;
    case SB_O:
	return "SB";
	break;
    case SW_O:
	return "SW";
	break;
    case JMP_O:
	return "JMP";
	break;
    case JAL_O:
	return "JAL";
	break;
    default:
	bail_with_error("Unknown op code (%d) in instruction_mnemonic!",
			i.immed.op);
	return NULL;
    }
    return NULL;
}

static char offset_comment_buf[512];

// return a comment string of the form
// "# offset is +/-d bytes"
static const char *instruction_offset_comment(short int o)
{
    sprintf(offset_comment_buf, "# offset is %+d bytes",
	    machine_types_formOffset(o));
    return strdup(offset_comment_buf);
}

// return a comment string of the form
// "# target is byte address %u"
static const char *instruction_formAddress_comment(unsigned int a)
{
    // We assume that the PC doesn't contribute to this address...
    // this makes sense for low addresses
    sprintf(offset_comment_buf, "# target is byte address %u",
	    BYTES_PER_WORD * a);
    return strdup(offset_comment_buf);
}


// Return a string containing the assembly langauge form of instr
const char *instruction_assembly_form(bin_instr_t instr) {
    char *buf = instr_buf;

    // put in the mnemonic for the instruction
    int cwr = sprintf(buf, "%s ", instruction_mnemonic(instr));
    // point buf to the null char that was printed into instr_buf
    buf += cwr;

    instr_type it = instruction_type(instr);
    switch (it) {
    case syscall_instr_type:
	// no arguments to these instructions, so nothing to do!
	break;
    case reg_instr_type:
	switch (instr.reg.func) {
	case ADD_F: case SUB_F: case AND_F: case BOR_F: case NOR_F: case XOR_F:
	    sprintf(buf, "%s, %s, %s",
		    regname_get(instr.reg.rs),
		    regname_get(instr.reg.rt),
		    regname_get(instr.reg.rd));
	    break;
	case MUL_F: case DIV_F:
	    sprintf(buf, "%s, %s",
		    regname_get(instr.reg.rs),
		    regname_get(instr.reg.rt));
	    break;
	case MFHI_F: case MFLO_F:
	    sprintf(buf, "%s", regname_get(instr.reg.rd));
	    break;
	case SLL_F: case SRL_F:
	    sprintf(buf, "%s, %s, %hu",
		    regname_get(instr.reg.rt),
		    regname_get(instr.reg.rd),
		    instr.reg.shift);
	    break;
	case JR_F:
	    sprintf(buf, "%s", regname_get(instr.reg.rs));
	    break;
	default:
	    bail_with_error("Unknown register instruction function (%d)!",
			    instr.reg.func);
	    break;
	}
	break;
    case immed_instr_type:
	switch (instr.immed.op) {
	case ADDI_O:
	    sprintf(buf, "%s, %s, %hd",
		    regname_get(instr.immed.rs),
		    regname_get(instr.immed.rt),
		    (short int) instr.immed.immed);
	    break;
	case ANDI_O: case BORI_O: case XORI_O:
	    sprintf(buf, "%s, %s, 0x%hx", // hex output most useful here
		    regname_get(instr.immed.rs),
		    regname_get(instr.immed.rt),
		    instr.immed.immed);
	    break;
	case BEQ_O: case BNE_O:
	    sprintf(buf, "%s, %s, %hd\t%s",
		    regname_get(instr.immed.rs),
		    regname_get(instr.immed.rt),
		    (short int) instr.immed.immed,
		    instruction_offset_comment((short int) instr.immed.immed));
	    break;
	case BGEZ_O: case BGTZ_O: case BLEZ_O: case BLTZ_O:
	    sprintf(buf, "%s, %hd\t%s", regname_get(instr.immed.rs),
		    (short int) instr.immed.immed,
		    instruction_offset_comment((short int) instr.immed.immed));
	    break;
	case LBU_O: case LW_O: case SB_O: case SW_O:
	    sprintf(buf, "%s, %s, %hd\t%s",
		    regname_get(instr.immed.rs),
		    regname_get(instr.immed.rt),
		    (short int) instr.immed.immed,
		    instruction_offset_comment((short int) instr.immed.immed));
	    break;
	default:
	    bail_with_error("Unknown immediate instruction opcode (%d)!",
			    instr.immed.op);
	    break;
	}
	break;
    case jump_instr_type:
	switch (instr.jump.op) {
	case JMP_O: case JAL_O:
	    sprintf(buf, "%u\t%s", instr.jump.addr,
		    instruction_formAddress_comment(instr.jump.addr));
	    break;
	default:
	    bail_with_error("Unknown jump instruction opcode (%d)!",
			    instr.jump.op);
	    break;
	}
	break;
    default:
	bail_with_error("Unknown instruction type (%d) in instruction_assembly_form!",
			it);
	break;
    }

    return instr_buf;
}

// Requires: out is open and writable FILE
// print the header of the instruction output table on out
void instruction_print_table_heading(FILE *out) {
    fprintf(out, "%-5s: %s\n", "Addr", "Instruction");
}

// Requires: out is an open FILE
// print addr on out, ": ", then the instruction's symbolic
// (assembly language) form, and finally a newline character (all on one line)
void instruction_print(FILE *out, address_type addr, bin_instr_t instr) {
    fprintf(out, "%-5u: %s\n", addr, instruction_assembly_form(instr));
}

// Check the sizes of the binary instruction types
// They all need to fit into a word (BYTES_PER_WORD bytes)
void instruction_check_sizes() {
    assert(sizeof(reg_instr_t) <= BYTES_PER_WORD);
    assert(sizeof(immed_instr_t) <= BYTES_PER_WORD);
    assert(sizeof(jump_instr_t) <= BYTES_PER_WORD);
    assert(sizeof(bin_instr_t) <= BYTES_PER_WORD);
}    
