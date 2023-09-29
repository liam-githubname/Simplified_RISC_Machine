// The header files aren't defining the symbols had to repalce with src
// files I know this is bad but I can't figure it out
// #include "bof.c"
// #include "utilities.c"
// #include "instruction.c"
// #include "regname.c"
// #include "machine_types.c"
// #include "parse_bof.c"
// #include "instruction.h"
// #include "machine_types.h"
// #include "utilities.h"
// #include "regname.h"
// #include "machine_types.h"
#include "instruction.h"
#include "bof.h"
#include "vm_mem.h"
#include "parse_bof.h"
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

    load_program(argv[2]);
    instruction_print_table_heading(stdout);

    int i;
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
    load_program(argv[1]);

    int i;
    instr_type cur_instr_type;

    for(i = 0; i < header.text_length / BYTES_PER_WORD; i++) {

      cur_instr_type = instruction_type(memory.instrs[i]);

      switch(cur_instr_type) {

        case reg_instr_type:

          switch (memory.instrs[i].reg.func) {

            case 33:
              registers[memory.instrs[i].reg.rd] = registers[memory.instrs[i].reg.rs] + registers[memory.instrs[i].reg.rt];
              break;

            case 35:
              registers[memory.instrs[i].reg.rd] = registers[memory.instrs[i].reg.rs] - registers[memory.instrs[i].reg.rt];
              break;
            case 25:
              //Multiplication will add later
              break;
            case 27:
              //Divide will add later
              break;
            case 16:
              registers[memory.instrs[i].reg.rd] = HI;
              break;
            case 18:
              registers[memory.instrs[i].reg.rd] = LO;
              break;
            case 36:
              registers[memory.instrs[i].reg.rd] = (registers[memory.instrs[i].reg.rs] & registers[memory.instrs[i].reg.rt]);
            case 37:
              registers[memory.instrs[i].reg.rd] = (registers[memory.instrs[i].reg.rs] | registers[memory.instrs[i].reg.rt]);
              break;
            case 39:
              registers[memory.instrs[i].reg.rd] = !(registers[memory.instrs[i].reg.rs] | registers[memory.instrs[i].reg.rt]);
              break;
            case 38:
              registers[memory.instrs[i].reg.rd] = (registers[memory.instrs[i].reg.rs] ^ registers[memory.instrs[i].reg.rt]);
              break;
            case 0:
              registers[memory.instrs[i].reg.rd] = registers[memory.instrs[i].reg.rt] << memory.instrs[i].reg.shift;
              break;
            case 3:
              registers[memory.instrs[i].reg.rd] = registers[memory.instrs[i].reg.rt] >> memory.instrs[i].reg.shift;
              break;
            case 8:
              // Jump will add later
              // fard
              break;
            case 12:
              // System call will add later;
              break;
          }
          break;

        case syscall_instr_type:
          break;



        
        //header.text_start_address
        case immed_instr_type:
        switch (memory.instrs[i].immed.op) {
          
          case 9:
            memory.instrs[i].immed.rt = memory.instrs[i].immed.rs + machine_types_sgnExt(memory.instrs[i].immed.immed);
          break;
          case 12:
            memory.instrs[i].immed.rt = memory.instrs[i].immed.rs & machine_types_zeroExt(memory.instrs[i].immed.immed);
          break;
          case 13:
            memory.instrs[i].immed.rt = memory.instrs[i].immed.rs | machine_types_zeroExt(memory.instrs[i].immed.immed);
          break;
          case 14:
            memory.instrs[i].immed.rt = memory.instrs[i].immed.rs ^ machine_types_zeroExt(memory.instrs[i].immed.immed);
          break;
          case 4:
            if(memory.instrs[i].immed.rs == memory.instrs[i].immed.rt)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 1:
            if(memory.instrs[i].immed.rs >= 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 7:
            if(memory.instrs[i].immed.rs > 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 6:
            if(memory.instrs[i].immed.rs <= 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 8:
            if(memory.instrs[i].immed.rs < 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 5:
            if(memory.instrs[i].immed.rs != memory.instrs[i].immed.rt)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 36:
            memory.instrs[i].immed.rt = machine_types_zeroExt(memory.bytes[memory.instrs[i].immed.rs + machine_types_formOffset(memory.instrs[i].immed.immed)]);
          break;
          case 35:
            memory.instrs[i].immed.rt = memory.bytes[memory.instrs[i].immed.rs + machine_types_formOffset(memory.instrs[i].immed.immed)];
          break;
          case 40:
            memory.bytes[memory.instrs[i].immed.rs + machine_types_formOffset(memory.instrs[i].immed.immed)] = memory.instrs[i].immed.rt;
            break;
          case 43:
      memory.bytes[memory.instrs[i].immed.rs + machine_types_formOffset(memory.instrs[i].immed.immed)] = memory.instrs[i].immed.rt;
            break;
        }
          break;
          
        case jump_instr_type:
          break;

        case error_instr_type:
          break;

        default:
          break;
      }
    }
  }
}
