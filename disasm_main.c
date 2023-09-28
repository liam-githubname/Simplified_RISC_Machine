/* $Id: disasm_main.c,v 1.3 2023/09/16 12:32:30 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include "bof.h"
#include "disasm.h"
#include "utilities.h"

static char *progname;

void usage() {
    bail_with_error("Usage: %s file.bof", progname);
}

int main(int argc, char *argv[]) {
    // set the program's name
    progname = argv[0];
    argc--;
    argv++;

    if (argc != 1) {
	usage();
    }

    // name of the file to read
    const char *bofname = argv[0];
    
    BOFFILE bf = bof_read_open(bofname);

    disasmProgram(stdout, bf);
    
    return EXIT_SUCCESS;
}
