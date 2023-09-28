// $Id: pass1.h,v 1.2 2023/09/14 21:19:33 leavens Exp $
#ifndef _PASS1_H
#define _PASS1_H
#include <stdio.h>
#include "ast.h"
#include "machine_types.h"

// Build the symbol table and check for duplicate declarations in the given AST
extern void pass1(program_t progast);

// Check the given AST and put its declarations in the symbol table
extern void pass1TextSection(text_section_t ts);

// Check the given AST and put its declarations in the symbol table
extern void pass1AsmInstrs(asm_instrs_t instrs);

// Check the given AST and put its declarations in the symbol table
extern void pass1AsmInstr(asm_instr_t instr, address_type count);

// Check the given AST and put its declarations in the symbol table
extern void pass1LabelOpt(label_opt_t lopt, address_type count);

// Check the given AST and put its declarations in the symbol table
extern void pass1DataSection(data_section_t ds);

// Check the given AST and put its declarations in the symbol table
extern void pass1StaticDecls(static_decls_t sds);

// Check the given AST and put its declarations in the symbol table
extern void pass1StaticDecl(static_decl_t dcl, address_type offset);

// Check the given AST and put its declarations in the symbol table
extern void pass1Ident(ident_t id, address_type offset);

// Requires: out is a character file (like stdout)
//           that is open for writing
// Print debugging information about the symbol table on out
void pass1_print(FILE *out);

#endif
