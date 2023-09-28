/* $Id: parser_types.h,v 1.5 2023/09/09 20:45:49 leavens Exp $ */
#ifndef _PARSER_TYPES_H
#define _PARSER_TYPES_H

#include "ast.h"

// The type of Bison's parser stack elements (parse values)
typedef AST YYSTYPE;
#define YYSTYPE_IS_DECLARED 1

#endif
