/* $Id: assemble.h,v 1.3 2023/09/15 14:21:40 leavens Exp $ */
#ifndef _ASSEMBLE_H
#define _ASSEMBLE_H
#include <stdio.h>
#include "ast.h"
#include "bof.h"

// Generate code for prog, with output going to the file out
extern void assembleProgram(BOFFILE bf, program_t prog);

// Unparse the given AST, with output going to bf
extern void assembleTextSection(BOFFILE bf, text_section_t ts);

// Unparse the given AST, with output going to bf
extern void assembleEntryPoint(BOFFILE bf, lora_t ep);

// Unparse the given AST, with output going to bf
extern void assembleLora(BOFFILE bf, lora_t l);

// Unparse the given AST, with output going to bf
extern void assembleAsmInstrs(BOFFILE bf, asm_instrs_t instrs);

// Unparse the given AST, with output going to bf
extern void assembleAsmInstr(BOFFILE bf, asm_instr_t instr);

// Unparse the given AST, with output going to bf
extern void assembleLabelOpt(BOFFILE bf, label_opt_t lopt);

// Unparse the given AST, with output going to bf
extern void assembleInstr(BOFFILE bf, instr_t instr);

// Unparse a register, with output going to bf
extern void assembleReg(BOFFILE bf, short unsigned n);

// Unparse the given AST, with output going to bf
extern void assembleDataSection(BOFFILE bf, data_section_t ds);

// Unparse the given AST, with output going to bf
extern void assembleStaticDecls(BOFFILE bf, static_decls_t sds);

// Unparse the given AST, with output going to bf
extern void assembleStaticDecl(BOFFILE bf, static_decl_t dcl);

// Unparse the given AST, with output going to bf
extern void assembleIdent(BOFFILE bf, ident_t id);

// Unparse the given AST, with output going to bf
extern void assembleInitializer(BOFFILE bf, initializer_t init);

#endif
