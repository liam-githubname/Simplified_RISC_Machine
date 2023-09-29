#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "vm_mem.h"
#include "parse_bof.h"
#include "bof.h"

// Formats the memory for the program and returns the mem_u variable containing it
union mem_u load_program(const char* filename) {

  BOFFILE bf = bof_read_open(filename);

  BOFHeader bf_header = bof_read_header(bf);

  int i;
  for(i = 0; i < bf_header.text_length / BYTES_PER_WORD; i++) {

    memory.instrs[i] = instruction_read(bf);

  }

  for(int j = 0; j < (bf_header.data_length / BYTES_PER_WORD); j++) {

    memory.words[i + j] = bof_read_word(bf);

  }

  return memory;

}
