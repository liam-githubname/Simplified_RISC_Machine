/* $Id: lexer.h,v 1.7 2023/09/17 20:21:27 leavens Exp $ */
#ifndef _LEXER_H
#define _LEXER_H
#include <stdio.h>
#include <stdbool.h>
#include "ast.h"
#include "parser_types.h"
#include "asm.tab.h"
#include "instruction.h"

// The current input file
extern FILE *yyin;
// The input line number of the current or next input line
extern int yylineno;
// The value of the current token
extern YYSTYPE yylval;

// Requires: fname != NULL
// Requires: fname is the name of a readable file
// Initialize the lexer and start it reading
// from the given file name
extern void lexer_init(const char *fname);

// Is the lexer's token stream finished
// (either at EOF or not open)?
extern bool lexer_done();

// Requires: !lexer_done()
// Return the next token in the input
extern int yylex();

// Requires: !lexer_done()
// Return the name of the current file
extern const char *lexer_filename();

// Requires: !lexer_done()
// Return the line number of the next token
extern unsigned int lexer_line();

// On standard output, print each token
// using the format in lexer_print_token
extern void lexer_output();

// Requires: toknum is a token number (from asm.tab.h)
//           of an instruction
// Return the opcode corresponding to the given opcode token number
extern op_code lexer_token2opcode(int toknum);

// Return the function code corresponding to the given opcode token number
extern func_code lexer_token2func(int toknum);

// Requires: toknum is a token number (from asm.tab.h)
//           that corresponds to a system call instruction
// Return the system call code that corresponds to that token
extern syscall_type lexer_token2syscall_code(int toknum);

#endif
