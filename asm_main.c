/* $Id: asm_main.c,v 1.17 2023/09/17 20:21:27 leavens Exp $ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "parser_types.h"
#include "bof.h"
#include "lexer.h"
#include "asm.tab.h"
#include "instruction.h"
#include "utilities.h"
#include "asm_unparser.h"
#include "pass1.h"
#include "assemble.h"

// strdup seems to be in the string library but not in the header...
extern char *strdup(const char *s);

/* The program's AST, set by the parser */
extern program_t progast;

// Requires: fn is a name that ends in .asm
// Modify fn to have the extension .bof
static void change_to_bof_ext(char *fn) {
    int len = strlen(fn);
    if (len <= 3) {
	bail_with_error("Not enough chars in file name passed to chage_to_bof_ext (\"%s\")",
			fn);
    }
    // assert(len > 3);
    char *endstr = strrchr(fn, '.');
    // assert(*endstr == '.');
    if (strcmp(endstr, ".asm") != 0) {
	bail_with_error("Name passed to change_to_bof_ext does not end in .asm (\"%s\")",
			fn);
    }
    strcpy(endstr, ".bof");
}

char * cmdname;
static const char *typicalFile = "file.asm";

void usage() {
    bail_with_error("Usage: %s %s\n       %s %s %s\n       %s %s %s\n       %s %s %s",
		    cmdname, typicalFile,
		    cmdname, "-l", typicalFile,
		    cmdname, "-u", typicalFile,
		    cmdname, "-s", typicalFile);
    exit(EXIT_FAILURE);
}

extern int yydebug;

int main(int argc, char *argv[]) {
    // for debugging, check sizes
    instruction_check_sizes();

    // should the tokens seen by the lexer be printed?
    bool lexer_print_output = false;
    // should the unparse be printed?
    bool parser_unparse = false;
    // should the symbol table be printed after pass 1?
    bool symbol_table_print = false;

    cmdname = argv[0];
    argc--;
    argv++;

    // possible options: -l, -u, and -s
    while (argc > 0 && strlen(argv[0]) >= 2 && argv[0][0] == '-') {
	if (strcmp(argv[0],"-l") == 0) {
	    lexer_print_output = true;
	    argc--;
	    argv++;
	} else if (strcmp(argv[0],"-u") == 0) {
	    parser_unparse = true;
	    argc--;
	    argv++;
	} else if (strcmp(argv[0],"-s") == 0) {
	    symbol_table_print = true;
	    argc--;
	    argv++;
	} else {
	    // bad option!
	    usage();
	}
    }

    // give usage message if -l and other options are used
    if ( lexer_print_output && (symbol_table_print || parser_unparse) ) {
	usage();
    }

    // give usage message if -u and other options are used
    if ( parser_unparse && symbol_table_print ) {
	usage();
    }

    // must have a file name
    if (argc <= 0 || (strlen(argv[0]) >= 2 && argv[0][0] == '-')) {
	usage();
    }

    // the name of the file
    const char *file_name = argv[0];

    if (lexer_print_output) {
	// with the lexer_print_output option, nothing else is done
	lexer_init(file_name);
	lexer_output();
	return EXIT_SUCCESS;
    }

    // otherwise (if not lexer_print_outout) continue to parse etc.
    lexer_init(file_name);
    int parser_ret = yyparse(file_name);
    if (parser_ret != 0) {
	exit(EXIT_FAILURE);
    }

    if (parser_unparse) {
	unparseProgram(stdout, progast);
    }

    if (parser_unparse) {
	return EXIT_SUCCESS;
    }

    // check for duplicate declarations of labels/names and build symbol table
    pass1(progast);

    // print debugging information about the symbol table
    if (symbol_table_print) {
	pass1_print(stdout);
    }

    char *bfn = strdup(file_name);
    change_to_bof_ext(bfn);
    
    BOFFILE bf = bof_write_open(bfn);

    // generate code from the ASTs
    assembleProgram(bf, progast);
    bof_close(bf);

    return EXIT_SUCCESS;
}
