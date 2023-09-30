#ifndef _VM_MEM_H
#define _VM_MEM_H


#include "bof.h"
#include "instruction.h"
#include "machine_types.h"

#define MEMORY_SIZE_IN_BYTES (65536 - BYTES_PER_WORD)
#define MEMORY_SIZE_IN_WORDS (MEMORY_SIZE_IN_BYTES / BYTES_PER_WORD)


union mem_u {

  byte_type bytes[MEMORY_SIZE_IN_BYTES];
  word_type words[MEMORY_SIZE_IN_WORDS];
  bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];

};

extern union mem_u memory;
extern word_type registers[32];
extern word_type HI, LO;

extern void print_cur_register(word_type registers[32], BOFHeader header, bin_instr_t instruction);

#endif
