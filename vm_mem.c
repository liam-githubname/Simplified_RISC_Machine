#include "bof.h"
#include "instruction.h"
#include "machine_types.h"
#include "regname.h"
#include "vm_mem.h"

union mem_u memory;

word_type HI = 0;
word_type LO = 0;

word_type registers[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void print_cur_register(word_type registers[32], BOFHeader header, bin_instr_t instruction) {


  printf("     PC: %d", header.text_start_address);
  for(int i = 0; i < 32; i++) {
    if (i % 6 == 0) printf("\n");
    printf("GPR[%s]: %d\t", regname_get(i), registers[i]);
  }

  printf("\n%d: %d\t...", registers[28], memory.bytes[registers[28]/BYTES_PER_WORD]);
  printf("\n%d: %d\t...", registers[29], memory.bytes[registers[29]/BYTES_PER_WORD]);
  printf("\n==> addr:\t%d %s\n", header.text_start_address, instruction_assembly_form(instruction));



}

