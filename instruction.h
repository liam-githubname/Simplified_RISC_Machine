// $Id: instruction.h,v 1.17 2023/09/18 02:24:31 leavens Exp $
#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H
#include <stdio.h>
#include <stdbool.h>
#include "machine_types.h"
#include "bof.h"

// op codes in binary instructions for the SRM
typedef enum {REG_O = 0, ADDI_O = 9, ANDI_O = 12, BORI_O = 13, XORI_O = 14,
	      BEQ_O = 4, BGEZ_O = 1, BGTZ_O = 7, BLEZ_O = 6, BLTZ_O = 8,
	      BNE_O = 5, LBU_O = 36, LW_O = 35, SB_O = 40, SW_O = 43,
	      JMP_O = 2, JAL_O = 3} op_code;

// function codes in binary instructions for the SRM (when opcode is 0)
typedef enum {ADD_F = 33, SUB_F = 35, MUL_F = 25, DIV_F = 27,
    MFHI_F = 16, MFLO_F = 18, AND_F = 36, BOR_F = 37, NOR_F = 39, XOR_F = 38,
    SLL_F = 0, SRL_F = 3, JR_F = 8, SYSCALL_F = 12} func_code;

// instruction types
typedef enum {reg_instr_type, syscall_instr_type, immed_instr_type,
    jump_instr_type, error_instr_type} instr_type;

// system calls
typedef enum {exit_sc = 10, print_str_sc = 4, print_char_sc = 11,
	      read_char_sc = 12, start_tracing_sc = 256, stop_tracing_sc = 257
} syscall_type;

// register/computational type instructions, except system calls
typedef struct {
    unsigned short op : 6;  // opcode, 6 bits
    reg_num_type rs : 5;  // source (argument) register, 5 bits
    reg_num_type rt : 5;  // second argument register, 5 bits
    reg_num_type rd : 5;  // destination register, 5 bits
    shift_type shift : 5; // shift amount, 5 bits
    func_type func : 6; // type of instruction, 6 bits
} reg_instr_t;

// system call instructions
typedef struct {
    unsigned short op : 6;  // opcode, 6 bits
    unsigned int code : 20;  // code for the system call, 20 bits
    func_type func : 6; // type of instruction, 6 bits
} syscall_instr_t;

// immediate type instructions
typedef struct {
    unsigned short op : 6;  // opcode, 6 bits
    reg_num_type rs : 5;  // source register, 5 bits
    reg_num_type rt : 5;  // target register, 5 bits
    immediate_type immed : 16; // immediate value, 16 bits
} immed_instr_t;

// jump type instructions
typedef struct {
    unsigned short op : 6;  // opcode, 6 bits
    address_type addr : 26; // target address, 26 bits
} jump_instr_t;

// binary instructions
typedef union {
    reg_instr_t reg;
    syscall_instr_t syscall;
    immed_instr_t immed;
    jump_instr_t jump;
} bin_instr_t;

// instructions as words (e.g., in a binary file)
typedef union wordAsInstr_u {
    word_type w;
    bin_instr_t bi;
} wordAsInstr_t;

// Return the type of the instruction given
extern instr_type instruction_type(bin_instr_t i);

// Requires: bof is open for reading in binary
// Read a single instruction (in binary) from bf and return it,
// but exit with an error if there is a problem.
extern bin_instr_t instruction_read(BOFFILE bf);

// Requires: bof is open for writing in binary
// Write the register instruction ri to bf in binary,
// but exit with an error if there is a problem.
extern void instruction_write_regInstr(BOFFILE bf, reg_instr_t ri);

// Requires: bof is open for writing in binary
// Write the system instruction si to bf in binary,
// but exit with an error if there is a problem.
extern void instruction_write_syscallInstr(BOFFILE bf, syscall_instr_t si);

// Requires: bof is open for writing in binary
// Write the immediate instruction ii with opcode op to bf in binary,
// but exit with an error if there is a problem.
extern void instruction_write_immedInstr(BOFFILE bf, unsigned short op,
					 immed_instr_t ii);

// Requires: bof is open for writing in binary
// Write the jump instruction ji with opcode op to bf in binary,
// but exit with an error if there is a problem.
extern void instruction_write_jumpInstr(BOFFILE bf, unsigned short op,
					jump_instr_t ji);

// Return the assembly language name (mnemonic) for i
extern const char *instruction_mnemonic(bin_instr_t i);

// Return a string containing the assembly langauge form of instr
extern const char *instruction_assembly_form(bin_instr_t instr);

// Requires: out is open and writable FILE
// print the header of the instruction output table on out
extern void instruction_print_table_heading(FILE *out);

// Requires: out is an open FILE
// print addr on out, ": ", then the instruction's symbolic
// (assembly language) form, and finally a newline character (all on one line)
extern void instruction_print(FILE *out, address_type addr, bin_instr_t instr);

// Requires: instr is a SYSCALL instruction
// (i.e., instr.op == 0 and instr.data.reg.func == SYSCALL_F).
// Return the code field that tells what kind of system call is being made
extern syscall_type instruction_syscall_number(bin_instr_t instr);

// Return the mnemonic for the given system call code
extern const char *instruction_syscall_mnemonic(unsigned int code);

// Given a binary instruction, bi, for a register format instruction
// return a string giving the assembly language mnemonic for it
extern const char *instruction_func2name(bin_instr_t bi);

// for debugging the type declarations
extern void instruction_check_sizes();

#endif
