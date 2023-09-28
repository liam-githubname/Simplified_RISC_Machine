/* $Id: asm_unparser.h,v 1.4 2023/09/16 13:45:07 leavens Exp $ */
#ifndef _ASM_UNPARSER_H
#define _ASM_UNPARSER_H
#include <stdio.h>
#include "ast.h"

// Unparse prog, with output going to the file out
extern void unparseProgram(FILE *out, program_t prog);

// Unparse the given AST, with output going to out
extern void unparseTextSection(FILE *out, text_section_t ts);

// Unparse the given AST, with output going to out
extern void unparseEntryPoint(FILE *out, lora_t ep);

// Unparse the given AST, with output going to out
extern void unparseLora(FILE *out, lora_t l);

// Unparse the given AST, with output going to out
extern void unparseAsmInstrs(FILE *out, asm_instrs_t instrs);

// Unparse the given AST, with output going to out
extern void unparseAsmInstr(FILE *out, asm_instr_t instr);

// Unparse the given AST, with output going to out
extern void unparseLabelOpt(FILE *out, label_opt_t lopt);

// Unparse the given AST, with output going to out
extern void unparseInstr(FILE *out, instr_t instr);

// Unparse a register, with output going to out
extern void unparseReg(FILE *out, unsigned short n);

// Unparse the given AST, with output going to out
extern void unparseDataSection(FILE *out, data_section_t ds);

// Unparse the given AST, with output going to out
extern void unparseStaticDecls(FILE *out, static_decls_t sds);

// Unparse the given AST, with output going to out
extern void unparseStaticDecl(FILE *out, static_decl_t dcl);

// Unparse the given AST, with output going to out
extern void unparseIdent(FILE *out, ident_t id);

// Unparse the given AST, with output going to out
extern void unparseInitializer(FILE *out, initializer_t init);

// Unparse the given AST, with output going to out
extern void unparseStackSection(FILE *out, stack_section_t ss);

#endif
