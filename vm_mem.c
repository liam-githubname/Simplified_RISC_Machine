#include "machine_types.h"
#include "regname.h"
#include "vm_mem.h"

union mem_u memory;

word_type HI, LO;

word_type registers[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void print_cur_register(word_type registers[32]) {

  for(int i = 0; i < 32; i++) {
    if (i % 6 == 0) printf("\n");
    printf("GPR[%s]: %d\t", regname_get(i), registers[i]);
  }

}

