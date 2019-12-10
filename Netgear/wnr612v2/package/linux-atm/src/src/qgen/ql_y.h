/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_BREAK = 258,
     TOK_CASE = 259,
     TOK_DEF = 260,
     TOK_DEFAULT = 261,
     TOK_LENGTH = 262,
     TOK_MULTI = 263,
     TOK_RECOVER = 264,
     TOK_ABORT = 265,
     TOK_ID = 266,
     TOK_INCLUDE = 267,
     TOK_STRING = 268
   };
#endif
#define TOK_BREAK 258
#define TOK_CASE 259
#define TOK_DEF 260
#define TOK_DEFAULT 261
#define TOK_LENGTH 262
#define TOK_MULTI 263
#define TOK_RECOVER 264
#define TOK_ABORT 265
#define TOK_ID 266
#define TOK_INCLUDE 267
#define TOK_STRING 268




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 142 "ql_y.y"
typedef union YYSTYPE {
    const char *str;
    int num;
    FIELD *field;
    VALUE *value;
    VALUE_LIST *list;
    TAG *tag;
    NAME_LIST *nlist;
} YYSTYPE;
/* Line 1285 of yacc.c.  */
#line 73 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



