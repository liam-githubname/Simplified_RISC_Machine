#include "bof.h"
#include "vm_mem.h"
#include "parse_bof.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc == 2) {

    BOFFILE bf = bof_read_open(argv[1]);



  }


  return EXIT_SUCCESS;
}
