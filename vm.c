#include "instruction.h"
#include "bof.h"
#include "vm_mem.h"
#include "parse_bof.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {

  if (argc == 0 || argc > 3) return EXIT_FAILURE;
  int flag=0;
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

    printf("%d:  0", header.data_start_address);
    printf("  ...\n");

    return EXIT_SUCCESS;

  } else {

    bf = bof_read_open(argv[1]);
    header = bof_read_header(bf);
    registers[29] = registers[30] = header.stack_bottom_addr;
    registers[28] = header.data_start_address;
    load_program(argv[1]);

    int i;
    instr_type cur_instr_type;

    for(i = 0; i < header.text_length / BYTES_PER_WORD; i++) {

      if(flag==0)
        print_cur_register(registers, header, memory.instrs[i]);

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
              // Multiplication will add later
                HI= (registers[memory.instrs[i].reg.rs] * registers[memory.instrs[i].reg.rt])<< 31;
                LO = (registers[memory.instrs[i].reg.rs] * registers[memory.instrs[i].reg.rt]);
              break;
            case 27:
              //Divide will add later
              HI = registers[memory.instrs[i].reg.rs] % registers[memory.instrs[i].reg.rt];
              LO = registers[memory.instrs[i].reg.rs] / registers[memory.instrs[i].reg.rt];
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
              // Jump need to test if this is working properly
              i = (registers[31]/4)-1;
              header.text_start_address = i*4;
              break;

            case 12:
              //TODO: System call will add later;
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
                   registers[2]=printf("%s",&memory.bytes[registers[4]]);
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
                     //TODO: test if this is working, pretty sure that it is
              flag=0;
              break;
            case 257://NOTR
                     //TODO: same comment as above obv
              flag=1;
              break;

          }
          break;




          //header.text_start_address
        case immed_instr_type:
          switch (memory.instrs[i].immed.op) {

            case 9:  //ADDI

              registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] + machine_types_sgnExt(memory.instrs[i].immed.immed);

              break;
            case 12: //ANDI
              registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] & machine_types_zeroExt(memory.instrs[i].immed.immed);
              break;
            case 13://BORI
              registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] | machine_types_zeroExt(memory.instrs[i].immed.immed);
              break;
            case 14://XORI
              registers[memory.instrs[i].immed.rt] = registers[memory.instrs[i].immed.rs] ^ machine_types_zeroExt(memory.instrs[i].immed.immed);
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
            case 2://JMP
              //Jump: PC ← formAddress(P C, a)
              //TODO: This will likely have the same problem as the JAL below
              i = (machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr)/4)-1;
              header.text_start_address=i*4;
              //header.text_start_address = machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr);
              break;

            case 3://JAL
              //Jump and Link: GPR[$ra] ← PC; PC ← formAddress(PC, a)
              //$ra technically index 31
              // TODO: This jump is for sure not working right, the next instruction read doesn't jump with the PC

              registers[31]= (header.text_start_address)+4;
              i = (machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr)/4);
              // = machine_types_formAddress(header.text_start_address, memory.instrs[i].jump.addr);
              i-=1;
              header.text_start_address = i*4;
              //header.text_start_address -= 4;
              //printf("****** %d",i);
              break;
          }
          break;

        case error_instr_type:
          // TODO: not sure what this should do
          break;

        default:
          break;
      }

      header.text_start_address += 4;

    }
  }
}
