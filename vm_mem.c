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


  printf("      PC: %d", header.text_start_address);
  if (HI > 0 || LO >0){
    printf("	       HI: %d	      LO: %d",HI,LO);
  }
  for(int i = 0; i < 32; i++) {
    if (i % 6 == 0) printf("\n");
    printf("GPR[%-3s]: %-4d\t", regname_get(i), registers[i]);

  }

  word_type temp_mem_buf = registers[28];

  for(int i = 0 ; i <= header.data_length / BYTES_PER_WORD; i++) {
    if (i % 5 == 0) printf("\n");

    if (memory.words[header.text_length / BYTES_PER_WORD + i] != 0) {

      printf("    %4d: %d ", temp_mem_buf, memory.words[header.text_length /BYTES_PER_WORD+i]);
      temp_mem_buf += 4;

    }

  }

  printf("     %d: 0", temp_mem_buf);
  printf("\t...");

  temp_mem_buf = registers[29];

  int printed_first = 0;
  int num_prints = 0;

  for(int i = 0 ; i < ((header.stack_bottom_addr - registers[29]) / BYTES_PER_WORD) + 1 ; i++) {

    if (num_prints % 5 == 0) printf("\n");

    if (memory.words[temp_mem_buf] == 0 && printed_first == 0) {
      printf("    %4d: %d\t...", temp_mem_buf, memory.words[temp_mem_buf]);
      printed_first = 1;
      num_prints++;
    }

    if (memory.words[temp_mem_buf] != 0) {
      printf("    %4d: %d ", temp_mem_buf, memory.words[temp_mem_buf]);
      printed_first = 0;
      num_prints++;
    }

    temp_mem_buf += 4;
  }

  // printf("     %d: 0", temp_mem_buf);
  // printf("\t...");


  printf("\n==> addr:%5d %-18s\n", header.text_start_address, instruction_assembly_form(instruction));



}
