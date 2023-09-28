 /* $Id: asm.y,v 1.34 2023/09/22 15:43:11 leavens Exp $ */

%code top {
#include <stdio.h>
}

%code requires {

 /* Including "ast.h" must be at the top, to define the AST type */
#include "ast.h"
#include "machine_types.h"
#include "parser_types.h"
#include "lexer.h"

    /* Report an error to the user on stderr */
extern void yyerror(const char *filename, const char *msg);
}    

%verbose
%define parse.lac full
%define parse.error detailed

%parse-param { char const *file_name }

%token eolsym
%token <ident> identsym
%token <unsignednum> unsignednumsym
%token <token> plussym    "+"
%token <token> minussym   "-"
%token commasym   ","
%token <token> dottextsym ".text"
%token <token> dotdatasym ".data"
%token <token> dotstacksym ".stack"
%token dotendsym  ".end"
%token colonsym   ":"
%token <token> equalsym   "="
%token <token> addopsym   "ADD"
%token <token> subopsym   "SUB"
%token <token> andopsym   "AND"
%token <token> boropsym   "BOR"
%token <token> noropsym   "NOR"
%token <token> xoropsym   "XOR"
%token <token> mulopsym   "MUL"
%token <token> divopsym   "DIV"
%token <token> sllopsym   "SLL"
%token <token> srlopsym   "SRL"
%token <token> mfhiopsym  "MFHI"
%token <token> mfloopsym  "MFLO"
%token <token> jropsym    "JR"
%token <token> addiopsym  "ADDI"
%token <token> andiopsym  "ANDI"
%token <token> boriopsym   "BORI"
%token <token> xoriopsym  "XORI"
%token <token> beqopsym   "BEQ"
%token <token> bgezopsym  "BGEZ"
%token <token> bgtzopsym  "BGTZ"
%token <token> blezopsym  "BLEZ"
%token <token> bltzopsym  "BLTZ"
%token <token> bneopsym   "BNE"
%token <token> lbuopsym   "LBU"
%token <token> lwopsym    "LW"
%token <token> sbopsym    "SB"
%token <token> swopsym    "SW"
%token <token> jmpopsym   "JMP"
%token <token> jalopsym   "JAL"
%token <token> exitopsym    "EXIT"
%token <token> pstropsym    "PSTR"
%token <token> pchopsym     "PCH"
%token <token> rchopsym     "RCH"
%token <token> straopsym    "STRA"
%token <token> notropsym    "NOTR"

%token <reg> regsym

%token <token> wordsym    "WORD"

%type <program> program
%type <text_section> textSection
%type <lora> entryPoint
%type <asm_instrs> asmInstrs
%type <lora> lora
%type <empty> empty
%type <ident> label
%type <label_opt> labelOpt
%type <instr> instr
%type <asm_instr> asmInstr
%type <instr> threeRegInstr
%type <instr> twoRegInstr
%type <instr> shiftInstr
%type <instr> oneRegInstr
%type <instr> immedArithInstr
%type <instr> immedBoolInstr
%type <instr> branchTestInstr
%type <instr> loadStoreInstr
%type <instr> jumpInstr
%type <instr> syscallInstr
%type <token> threeRegOp
%type <token> twoRegOp
%type <token> shiftOp
%type <unsignednum> shift
%type <token> moveHiLoOp
%type <token> jrOp
%type <token> immedArithOp
%type <number> immed
%type <number> number
%type <token> sign
%type <token> immedBoolOp
%type <token> branchTest2Op
%type <token> branchTest1Op
%type <number> offset
%type <token> loadStoreOp
%type <token> jumpOp
%type <token> syscallOp
%type <data_section> dataSection
%type <unsignednum> staticStartAddr
%type <static_decls> staticDecls
%type <static_decl> staticDecl
%type <data_size> dataSize
%type <initializer> initializerOpt
%type <stack_section> stackSection
%type <unsignednum> stackBottomAddr

%start program

%code {
 /* extern declarations provided by the lexer */
extern int yylex(void);

 /* extern void yyerror(char const *msg); */

 /* The AST for the program, set by the semantic action for program. */
program_t progast; 

 /* Set the program's ast to be t */
extern void setProgAST(program_t t);
}

%%

program : textSection dataSection stackSection ".end"
          { setProgAST(
                       ast_program($1,
                                   $2,
				   $3
                                  )
                       );
          }
          ;

textSection : ".text" entryPoint asmInstrs 
            { $$ = ast_text_section($1,$2,$3); }
          ;

entryPoint : lora ;

lora : label { $$ = ast_lora_label($1); }
     | unsignednumsym { $$ = ast_lora_addr($1); }
     ;

asmInstrs : asmInstr { $$ = ast_asm_instrs_singleton($1); }
          | asmInstrs asmInstr { $$ = ast_asm_instrs_add($1,$2); }
          ;

label : identsym ;

asmInstr : labelOpt instr eolsym { $$ = ast_asm_instr($1,$2); } ;

labelOpt : label ":" { $$ = ast_label_opt_label($1); }
         | empty { $$ = ast_label_opt_empty($1); }
         ;

empty : %empty { $$ = ast_empty(lexer_filename(), lexer_line()); } ;

instr : threeRegInstr | twoRegInstr | shiftInstr | oneRegInstr
      | immedArithInstr | immedBoolInstr | branchTestInstr 
      | loadStoreInstr | jumpInstr | syscallInstr
      ;

threeRegInstr : threeRegOp regsym "," regsym "," regsym
                {
		    $$ = ast_instr($1, reg_instr_type, 3, $2.number,
				   $4.number, $6.number,
				   lexer_token2func($1.code),
				   ik_none, ast_immed_none());
		}
              ;

threeRegOp : "ADD" | "SUB" | "AND" | "BOR" | "NOR" | "XOR" ;

twoRegInstr : twoRegOp regsym "," regsym
              {
		  $$ = ast_instr($1, reg_instr_type, 2, $2.number,
				 $4.number, 0,
				 lexer_token2func($1.code),
				 ik_none, ast_immed_none());
              }
              ;

twoRegOp : "MUL" | "DIV" ;

shiftInstr : shiftOp regsym "," regsym "," shift
           {
	       // say we are using 3 registers,
	       // because the registers used are rt and rd, not rs
	       $$ = ast_instr($1, reg_instr_type, 3, 0, $2.number, $4.number, 
			      lexer_token2func($1.code),
			      ik_uimmed, ast_immed_unsigned($6.value));
	   }
           ;

shiftOp : "SLL" | "SRL" ;

shift : unsignednumsym ;

oneRegInstr : moveHiLoOp regsym
              {
		  // say we are using 3 registers,
		  // because the register number goes in rd, not rs
		  $$ = ast_instr($1, reg_instr_type, 3, 0, 0, $2.number,
				 lexer_token2func($1.code),
				 ik_none, ast_immed_none());
	      }
              | jrOp regsym
              {
		  $$ = ast_instr($1, reg_instr_type, 1, $2.number, 0, 0,
				 lexer_token2func($1.code),
				 ik_none, ast_immed_none());
	      }
              ;

moveHiLoOp : "MFHI" | "MFLO" ;

jrOp : "JR" ;

immedArithInstr : immedArithOp regsym "," regsym "," immed
                  {
		      $$ = ast_instr($1, immed_instr_type, 2, $2.number,
				     $4.number, 0, 0,
				     ik_immed, ast_immed_number($6.value));
		  }
                ;

immedArithOp : "ADDI" ;

immed : number ;

number : sign unsignednumsym
         {
	       word_type val = $2.value;
               if ($1.code == minussym) {
		   val = - val;
               }
               $$ = ast_number($1, val);
         }
         ;

sign : "+" | "-"
     | empty { $$ = ast_token(lexer_filename(), lexer_line(), plussym); }
     ; 

immedBoolInstr : immedBoolOp regsym "," regsym "," unsignednumsym
                 {
		     $$ = ast_instr($1, immed_instr_type, 2, $2.number,
				    $4.number, 0, 0,
				    ik_uimmed, ast_immed_unsigned($6.value));
		 }
               ;

immedBoolOp : "ANDI" | "BORI" | "XORI" ;

branchTestInstr : branchTest2Op regsym "," regsym "," offset
                  {
		      $$ = ast_instr($1, immed_instr_type, 2, $2.number,
				     $4.number, 0, 0,
				     ik_immed, ast_immed_number($6.value));
		  }
                | branchTest1Op regsym "," offset
		  {
		      $$ = ast_instr($1, immed_instr_type, 1, $2.number, 0, 0,
				     0, ik_immed, ast_immed_number($4.value));
		  }
                  ;

branchTest2Op : "BEQ" | "BNE" ;

branchTest1Op : "BGEZ" | "BGTZ" | "BLEZ" | "BLTZ" ;

offset : number ;

loadStoreInstr : loadStoreOp regsym "," regsym "," offset
                 {
		     $$ = ast_instr($1, immed_instr_type, 2, $2.number,
				    $4.number, 0, 0,
				    ik_immed, ast_immed_number($6.value));
		 }
               ;

loadStoreOp : "LBU" | "LW" | "SB" | "SW" ;


jumpInstr : jumpOp lora
            {
		$$ = ast_instr($1, jump_instr_type, 0, 0, 0, 0, 0,
			       ik_uimmed, ast_immed_lora($2));
	    }
            ;

jumpOp : "JMP" | "JAL" ;

syscallInstr : syscallOp
             {
		 immedData_t id;
		 id.id_data_kind = ik_syscall_code;
		 id.data.syscall_code = lexer_token2syscall_code($1.code);
		 $$ = ast_instr($1, syscall_instr_type, 0, 0, 0, 0,
				SYSCALL_F,
				ik_syscall_code, id);
	     }
             ;

syscallOp : "EXIT" | "PSTR" | "PCH" | "RCH" | "STRA" | "NOTR" ;

dataSection : ".data" staticStartAddr staticDecls
              { $$ = ast_data_section($1, $2.value, $3); }
              ;

staticStartAddr : unsignednumsym ;

staticDecls : empty { $$ = ast_static_decls_empty($1); }
            | staticDecls staticDecl { $$ = ast_static_decls_add($1,$2); }
            ;

staticDecl : dataSize identsym initializerOpt eolsym
             { $$ = ast_static_decl($1, $2, $3); }
             ;

dataSize : "WORD" { $$ = ast_data_size($1, BYTES_PER_WORD); }
         ;

initializerOpt : "=" number { $$ = ast_initializer_given($1, $2.value); }
               | empty { $$ = ast_initializer_empty($1); }
               ;

stackSection : ".stack" stackBottomAddr
              { $$ = ast_stack_section($1, $2.value); }
              ;

stackBottomAddr : unsignednumsym ;

%%

// Set the program's ast to be t
void setProgAST(program_t t) { progast = t; }
