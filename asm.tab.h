/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_ASM_TAB_H_INCLUDED
# define YY_YY_ASM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 7 "asm.y"


 /* Including "ast.h" must be at the top, to define the AST type */
#include "ast.h"
#include "machine_types.h"
#include "parser_types.h"
#include "lexer.h"

    /* Report an error to the user on stderr */
extern void yyerror(const char *filename, const char *msg);

#line 61 "asm.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    eolsym = 258,                  /* eolsym  */
    identsym = 259,                /* identsym  */
    unsignednumsym = 260,          /* unsignednumsym  */
    plussym = 261,                 /* "+"  */
    minussym = 262,                /* "-"  */
    commasym = 263,                /* ","  */
    dottextsym = 264,              /* ".text"  */
    dotdatasym = 265,              /* ".data"  */
    dotstacksym = 266,             /* ".stack"  */
    dotendsym = 267,               /* ".end"  */
    colonsym = 268,                /* ":"  */
    equalsym = 269,                /* "="  */
    addopsym = 270,                /* "ADD"  */
    subopsym = 271,                /* "SUB"  */
    andopsym = 272,                /* "AND"  */
    boropsym = 273,                /* "BOR"  */
    noropsym = 274,                /* "NOR"  */
    xoropsym = 275,                /* "XOR"  */
    mulopsym = 276,                /* "MUL"  */
    divopsym = 277,                /* "DIV"  */
    sllopsym = 278,                /* "SLL"  */
    srlopsym = 279,                /* "SRL"  */
    mfhiopsym = 280,               /* "MFHI"  */
    mfloopsym = 281,               /* "MFLO"  */
    jropsym = 282,                 /* "JR"  */
    addiopsym = 283,               /* "ADDI"  */
    andiopsym = 284,               /* "ANDI"  */
    boriopsym = 285,               /* "BORI"  */
    xoriopsym = 286,               /* "XORI"  */
    beqopsym = 287,                /* "BEQ"  */
    bgezopsym = 288,               /* "BGEZ"  */
    bgtzopsym = 289,               /* "BGTZ"  */
    blezopsym = 290,               /* "BLEZ"  */
    bltzopsym = 291,               /* "BLTZ"  */
    bneopsym = 292,                /* "BNE"  */
    lbuopsym = 293,                /* "LBU"  */
    lwopsym = 294,                 /* "LW"  */
    sbopsym = 295,                 /* "SB"  */
    swopsym = 296,                 /* "SW"  */
    jmpopsym = 297,                /* "JMP"  */
    jalopsym = 298,                /* "JAL"  */
    exitopsym = 299,               /* "EXIT"  */
    pstropsym = 300,               /* "PSTR"  */
    pchopsym = 301,                /* "PCH"  */
    rchopsym = 302,                /* "RCH"  */
    straopsym = 303,               /* "STRA"  */
    notropsym = 304,               /* "NOTR"  */
    regsym = 305,                  /* regsym  */
    wordsym = 306                  /* "WORD"  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (char const *file_name);


#endif /* !YY_YY_ASM_TAB_H_INCLUDED  */
