// The header files aren't defining the symbols had to repalce with src
// files I know this is bad but I can't figure it out
#include "bof.c"
#include "utilities.c"
#include "instruction.c"
#include "regname.c"
#include "machine_types.c"
// #include "parse_bof.c"
// #include "utilities.h"
// #include "regname.h"
// #include "machine_types.h"
// #include "instruction.h"
// #include "bof.h"
#include "vm_mem.h"
// #include "parse_bof.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {

  if (argc == 0 || argc > 3) return EXIT_FAILURE;

  BOFFILE bf;
  BOFHeader header;
  if (strcmp(argv[1], "-p") == 0) {

    bf = bof_read_open(argv[2]);
    header = bof_read_header(bf);

    // TODO: Having parse_bof.c is causing undefined symbol errors
    // I would like fix this issue
    // load_program(argv[2]);
    instruction_print_table_heading(stdout);

    int i;
    for(i = 0; i < header.text_length / BYTES_PER_WORD; i++) {

      memory.instrs[i] = instruction_read(bf);

    }

    for(int j = 0; j < (header.data_length / BYTES_PER_WORD); j++) {

      memory.words[i + j] = bof_read_word(bf);

    }

    i = 0;

    for (i = header.text_start_address; i < header.text_length / BYTES_PER_WORD; i++) {
      printf("%d \t%s\n", header.text_start_address, instruction_assembly_form(memory.instrs[i]));
      header.text_start_address += 4;
    }

    if (header.data_length > 0) {
      for (int j = 0; j < header.data_length / BYTES_PER_WORD; j++) {
        printf("%d:\t%d\t", header.data_start_address, memory.words[j + i]);
        header.data_start_address += 4;
      }
    }

    printf("%d: \t0", header.data_start_address);
    printf("\t ...\n");

    return EXIT_SUCCESS;
  } else {

    bf = bof_read_open(argv[1]);
    header = bof_read_header(bf);
    // load_program(argv[1]);


  }

}
