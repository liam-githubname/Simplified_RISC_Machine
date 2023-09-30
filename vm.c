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
              break;
            case 12:
              // System call will add later;
              break;
          }
          break;



        
        case syscall_instr_type:
          switch (memory.instrs[i].syscall.code) {

            case 10:// EXIT
              exit(0);
            break;
            
            case 4://PSTR
              //GPR[$v0]  is the index 2
              //GPR[$a0] is index 4 
              //GPR[$v0] ← printf("%s",&memory[GPR[$a0]])
             // registers[2]=printf("%s",&registers[4]);
            break;
            
            case 11://PCH
              //GPR[$v0] ←fputc(GPR[$a0],stdout)
              registers[2] = fputc(registers[4],stdout);
            break;
            
            case 12://RCH
              //GPR[$v0] ← getc(stdin)

              registers[2] = getc(stdin);
            break;
            
            case 256://STRA
              //TODO start VM tracing; start tracing output
            break;
            case 257://NOTR
              //TODO no VM tracing; stop the tracing output
            break;
            
          }
          break;



        
        //header.text_start_address
        case immed_instr_type:
        switch (memory.instrs[i].immed.op) {
          
          case 9:  //ADDI
            registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] + machine_types_sgnExt(registers[memory.instrs[i].immed.immed]);
          break;
          case 12: //ANDI
            registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] & machine_types_zeroExt(registers[memory.instrs[i].immed.immed]);
          break;
          case 13://BORI 
            registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] | machine_types_zeroExt(registers[memory.instrs[i].immed.immed]);
          break;
          case 14://XORI
            registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] ^ machine_types_zeroExt(registers[memory.instrs[i].immed.immed]);
          break;
          case 4://BEQ
            if(registers[memory.instrs[i].immed.rs] == registers[memory.instrs[i].immed.rt])
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 1://BGEZ
            if(registers[memory.instrs[i].immed.rs] >= 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 7: //BGTZ
            if(registers[memory.instrs[i].immed.rs] > 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 6://BLEZ
            if(registers[memory.instrs[i].immed.rs] <= 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 8:// BLTZ
            if(registers[memory.instrs[i].immed.rs] < 0)
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 5: //BNE
            if(registers[memory.instrs[i].immed.rs] != registers[memory.instrs[i].immed.rt])
              header.text_start_address = header.text_start_address + machine_types_formOffset(memory.instrs[i].immed.immed);
          break;
          case 36:  //LBU
            registers[memory.instrs[i].immed.rt] = machine_types_zeroExt(memory.bytes[registers[memory.instrs[i].immed.rs] + machine_types_formOffset(memory.instrs[i].immed.immed)]);
          break;
          case 35:  //LW
            registers[memory.instrs[i].immed.rt] = memory.bytes[registers[memory.instrs[i].immed.rs] + machine_types_formOffset(memory.instrs[i].immed.immed)];
          break;
          case 40:  //SB
            memory.bytes[registers[memory.instrs[i].immed.rs] + machine_types_formOffset(memory.instrs[i].immed.immed)] = memory.instrs[i].immed.rt;
            break;
          case 43:  //SW
      memory.bytes[registers[memory.instrs[i].immed.rs] + machine_types_formOffset(memory.instrs[i].immed.immed)] = memory.instrs[i].immed.rt;
            break;
        }
          break;
          
        case jump_instr_type:
          switch (memory.instrs[i].jump.op) {
            case 2:
              //Jump: PC ← formAddress(P C, a)
              header.text_start_address = machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr);
            break;
            
            case 3:
              //Jump and Link: GPR[$ra] ← PC; PC ← formAddress(PC, a)
              //$ra technically index 31
              registers[31]= header.text_start_address;
              header.text_start_address = machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr);
            break;
          }
          break;

        case error_instr_type:
          // not sure what this should do
          break;

        default:
          break;
      }
    }
  }
}
