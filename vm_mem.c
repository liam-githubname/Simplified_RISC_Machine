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
  if (HI > 0 || LO >0){
    printf("	       HI: %d	      LO: %d",HI,LO);
  }
  for(int i = 0; i < 32; i++) {
    if (i % 6 == 0) printf("\n");
    printf("GPR[%s]: %d   	", regname_get(i), registers[i]);
  }

  word_type temp_mem_buf = registers[28];

  printf("\n");
  for(int i = 0 ; i <header.data_length / BYTES_PER_WORD; i++) {
    printf("     %d: %d", temp_mem_buf, memory.words[header.text_length /BYTES_PER_WORD+i]);
    temp_mem_buf += 4;
  }

  printf("     %d: 0", temp_mem_buf);
  printf("\t ...");


  printf("\n    %d: %d	...", registers[29], memory.bytes[registers[29]/BYTES_PER_WORD]);
  printf("\n==> addr:\t%d %s\n", header.text_start_address, instruction_assembly_form(instruction));



}
