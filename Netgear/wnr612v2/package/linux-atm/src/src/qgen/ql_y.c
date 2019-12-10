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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "ql_y.y"

/* ql.y - Q.2931 data structures description language */

/* Written 1995-1997 by Werner Almesberger, EPFL-LRC */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "common.h"
#include "qgen.h"
#include "file.h"

extern void yyerror(const char *s);


#define MAX_TOKEN 256
#define DEFAULT_NAMELIST_FILE "default.nl"


FIELD *def = NULL;
static STRUCTURE *structures = NULL;
static const char *abort_id; /* indicates abort flag */


static NAME_LIST *get_name_list(const char *name)
{
    static NAME_LIST *name_lists = NULL;
    FILE *file;
    NAME_LIST *list;
    NAME *last,*this;
    char line[MAX_TOKEN+1];
    char path[PATH_MAX+1];
    char *start,*here,*walk;
    int searching,found;

    for (list = name_lists; list; list = list->next)
	if (list->list_name == name) return list;
    sprintf(path,"%s.nl",name);
    if (!(file = fopen(path,"r")) && !(file = fopen(strcpy(path,
      DEFAULT_NAMELIST_FILE),"r"))) yyerror("can't open list file");
    list = alloc_t(NAME_LIST);
    list->list_name = name;
    list->list = last = NULL;
    list->id = -1;
    list->next = name_lists;
    name_lists = list;
    searching = 1;
    found = 0;
    while (fgets(line,MAX_TOKEN,file)) {
	for (start = line; *start && isspace(*start); start++);
	if (!*start || *start == '#') continue;
	if ((here = strchr(start,'\n'))) *here = 0;
	for (walk = strchr(start,0)-1; walk > start && isspace(*walk); walk--)
	    *walk = 0;
	if (*start == ':') {
	    if (!(searching = strcmp(start+1,name))) {
		if (found) yyerror("multiple entries");
		else found = 1;
	    }
	    continue;
	}
	if (searching) continue;
	if (!(here = strchr(start,'='))) yyerror("invalid name list");
	*here++ = 0;
	for (walk = here-2; walk > start && isspace(*walk); walk--)
	    *walk = 0;
	while (*here && isspace(*here)) here++;
	this = alloc_t(NAME);
	this->value = stralloc(start);
	this->name = stralloc(here);
	this->next = NULL;
	if (last) last->next = this;
	else list->list = this;
	last = this;
    }
    (void) fclose(file);
    if (!found) yyerror("no symbol list entry found");
    return list;
}


static FIELD *copy_block(FIELD *orig_field)
{
    FIELD *copy,**new_field;

    copy = NULL;
    new_field = &copy;
    while (orig_field) {
	*new_field = alloc_t(FIELD);
	**new_field = *orig_field;
	if (orig_field->value) {
	    (*new_field)->value = alloc_t(VALUE);
	    *(*new_field)->value = *orig_field->value;
	    switch (orig_field->value->type) {
		case vt_length:
		    (*new_field)->value->block =
		      copy_block(orig_field->value->block);
		    break;
		case vt_case:
		case vt_multi:
		    {
			TAG *orig_tag,**new_tag;

			new_tag = &(*new_field)->value->tags;
			for (orig_tag = orig_field->value->tags; orig_tag;
			  orig_tag = orig_tag->next) {
			    VALUE_LIST *orig_value,**new_value;

			    *new_tag = alloc_t(TAG);
			    **new_tag = *orig_tag;
			    new_value = &(*new_tag)->more;
			    for (orig_value = orig_tag->more; orig_value;
			      orig_value = orig_value->next) {
				*new_value = alloc_t(VALUE_LIST);
				**new_value = *orig_value;
				new_value = &(*new_value)->next;
			    }
			    (*new_tag)->block = copy_block(orig_tag->block);
			    new_tag = &(*new_tag)->next;
			}
		    }
	    }
	}
	if (orig_field->structure)
	    yyerror("sorry, can't handle nested structures");
	new_field = &(*new_field)->next;
	orig_field = orig_field->next;
    }
    return copy;
}




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 253 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 265 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   65

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  23
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  25
/* YYNRULES -- Number of rules. */
#define YYNRULES  46
/* YYNRULES -- Number of states. */
#define YYNSTATES  86

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    21,    18,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    22,     2,
      17,    14,    19,     2,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    15,     2,    16,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     7,     8,    11,    12,    15,    20,    21,
      24,    26,    30,    33,    34,    37,    43,    44,    46,    50,
      56,    57,    60,    62,    63,    66,    67,    70,    72,    77,
      82,    86,    87,    90,    91,    93,    94,   100,   101,   108,
     109,   115,   116,   123,   124,   127,   128
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      24,     0,    -1,    25,    26,    30,    -1,    -1,    12,    25,
      -1,    -1,    26,    27,    -1,     5,    11,    14,    30,    -1,
      -1,    29,    30,    -1,    11,    -1,    15,    31,    16,    -1,
      10,    11,    -1,    -1,    32,    31,    -1,    33,    11,    41,
      17,    34,    -1,    -1,     3,    -1,    18,    36,    19,    -1,
      36,    35,    37,    19,    38,    -1,    -1,    20,    36,    -1,
      11,    -1,    -1,    21,    11,    -1,    -1,    14,    39,    -1,
      11,    -1,     4,    15,    42,    16,    -1,     8,    15,    44,
      16,    -1,    40,     7,    30,    -1,    -1,     9,    11,    -1,
      -1,    13,    -1,    -1,     6,    11,    46,    47,    30,    -1,
      -1,    11,    46,    47,    30,    43,    42,    -1,    -1,     6,
      11,    46,    47,    28,    -1,    -1,    11,    46,    47,    28,
      45,    44,    -1,    -1,    22,    11,    -1,    -1,    21,    11,
      47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   167,   167,   178,   179,   187,   188,   192,   206,   206,
     216,   234,   239,   247,   250,   258,   282,   285,   292,   304,
     323,   326,   334,   344,   347,   355,   358,   365,   371,   378,
     384,   395,   398,   405,   408,   415,   418,   436,   435,   459,
     462,   479,   478,   502,   505,   512,   515
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_BREAK", "TOK_CASE", "TOK_DEF",
  "TOK_DEFAULT", "TOK_LENGTH", "TOK_MULTI", "TOK_RECOVER", "TOK_ABORT",
  "TOK_ID", "TOK_INCLUDE", "TOK_STRING", "'='", "'{'", "'}'", "'<'", "'-'",
  "'>'", "'@'", "','", "':'", "$accept", "all", "includes", "structures",
  "structure", "rep_block", "@1", "block", "fields", "field", "opt_break",
  "field_cont", "opt_pos", "decimal", "opt_more", "opt_val", "value",
  "opt_recover", "opt_name_list", "tags", "@2", "rep_tags", "@3", "opt_id",
  "list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    61,   123,   125,    60,    45,    62,
      64,    44,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    23,    24,    25,    25,    26,    26,    27,    29,    28,
      30,    30,    30,    31,    31,    32,    33,    33,    34,    34,
      35,    35,    36,    37,    37,    38,    38,    39,    39,    39,
      39,    40,    40,    41,    41,    42,    42,    43,    42,    44,
      44,    45,    44,    46,    46,    47,    47
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     0,     2,     0,     2,     4,     0,     2,
       1,     3,     2,     0,     2,     5,     0,     1,     3,     5,
       0,     2,     1,     0,     2,     0,     2,     1,     4,     4,
       3,     0,     2,     0,     1,     0,     5,     0,     6,     0,
       5,     0,     6,     0,     2,     0,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     3,     0,     5,     4,     1,     0,     0,     0,    10,
      13,     6,     2,     0,    12,    17,     0,    13,     0,     0,
      11,    14,    33,     7,    34,     0,     0,    22,     0,    15,
      20,     0,     0,    23,    18,    21,     0,     0,    24,    25,
      31,    19,     0,     0,     0,    27,    26,     0,    35,    39,
      32,     0,     0,    43,     0,     0,    43,     0,    30,    43,
       0,    45,    28,    43,    45,    29,    45,    44,     0,     0,
      45,     8,     0,    45,    37,     8,    41,     0,    36,    46,
      35,    40,    39,     9,    38,    42
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     3,     6,    11,    76,    77,    12,    16,    17,
      18,    29,    33,    30,    37,    41,    46,    47,    25,    54,
      80,    57,    82,    61,    69
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -62
static const yysigned_char yypact[] =
{
      -8,    -8,    14,   -62,   -62,   -62,    -4,    16,    17,   -62,
      -1,   -62,   -62,    20,   -62,   -62,    21,    -1,    22,    11,
     -62,   -62,    23,   -62,   -62,    24,    -3,   -62,    27,   -62,
      25,    28,    27,    30,   -62,   -62,    29,    33,   -62,    32,
       9,   -62,    34,    39,    31,   -62,   -62,    36,    18,    19,
     -62,    11,    37,    35,    40,    44,    35,    43,   -62,    35,
      49,    41,   -62,    35,    41,   -62,    41,   -62,    50,    11,
      41,   -62,    11,    41,   -62,   -62,   -62,    11,   -62,   -62,
      18,   -62,    19,   -62,   -62,   -62
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -62,   -62,    38,   -62,   -62,   -31,   -62,   -19,    46,   -62,
     -62,   -62,   -62,     3,   -62,   -62,   -62,   -62,   -62,   -16,
     -62,   -17,   -62,   -40,   -61
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -17
static const yysigned_char yytable[] =
{
      23,     7,    15,    71,     1,    72,     8,     9,    27,    75,
     -16,    10,    79,    42,     5,    28,    64,    43,    44,    66,
      45,     8,     9,    70,    52,    55,    10,    13,    14,    53,
      56,    31,    58,    22,    19,    35,    24,    20,    27,     4,
      38,    26,    50,    51,    81,    32,    40,    34,    59,    48,
      74,    36,    39,    78,    49,    63,    62,    60,    83,    65,
      67,    73,    68,    21,    84,    85
};

static const unsigned char yycheck[] =
{
      19,     5,     3,    64,    12,    66,    10,    11,    11,    70,
      11,    15,    73,     4,     0,    18,    56,     8,     9,    59,
      11,    10,    11,    63,     6,     6,    15,    11,    11,    11,
      11,    28,    51,    11,    14,    32,    13,    16,    11,     1,
      11,    17,    11,     7,    75,    20,    14,    19,    11,    15,
      69,    21,    19,    72,    15,    11,    16,    22,    77,    16,
      11,    11,    21,    17,    80,    82
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    12,    24,    25,    25,     0,    26,     5,    10,    11,
      15,    27,    30,    11,    11,     3,    31,    32,    33,    14,
      16,    31,    11,    30,    13,    41,    17,    11,    18,    34,
      36,    36,    20,    35,    19,    36,    21,    37,    11,    19,
      14,    38,     4,     8,     9,    11,    39,    40,    15,    15,
      11,     7,     6,    11,    42,     6,    11,    44,    30,    11,
      22,    46,    16,    11,    46,    16,    46,    11,    21,    47,
      46,    47,    47,    11,    30,    47,    28,    29,    30,    47,
      43,    28,    45,    30,    42,    44
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 168 "ql_y.y"
    {
	    STRUCTURE *walk;

	    def = yyvsp[0].field;
	    for (walk = structures; walk; walk = walk->next)
		if (!walk->instances)
		    fprintf(stderr,"unused structure: %s\n",walk->id);
	}
    break;

  case 4:
#line 180 "ql_y.y"
    {
	    to_c("#%s\n",yyvsp[-1].str);
	    to_test("#%s\n",yyvsp[-1].str);
	    if (dump) to_dump("#%s\n",yyvsp[-1].str);
	}
    break;

  case 7:
#line 193 "ql_y.y"
    {
	    STRUCTURE *n;

	    n = alloc_t(STRUCTURE);
	    n->id = yyvsp[-2].str;
	    n->block = yyvsp[0].field;
	    n->instances = 0;
	    n->next = structures;
	    structures = n;
	}
    break;

  case 8:
#line 206 "ql_y.y"
    {
	    abort_id = NULL;
	}
    break;

  case 9:
#line 210 "ql_y.y"
    {
	    yyval.field = yyvsp[0].field;
	}
    break;

  case 10:
#line 217 "ql_y.y"
    {
	    STRUCTURE *walk;

	    for (walk = structures; walk; walk = walk->next)
		if (walk->id == yyvsp[0].str) break;
	    if (!walk) yyerror("no such structure");
	    walk->instances++;
	    yyval.field = alloc_t(FIELD);
	    yyval.field->id = NULL;
	    yyval.field->name_list = NULL;
	    yyval.field->value = NULL;
	    yyval.field->brk = 0;
	    yyval.field->structure = walk;
	    yyval.field->my_block = copy_block(walk->block);
	    yyval.field->next = NULL;
	    abort_id = NULL;
	}
    break;

  case 11:
#line 235 "ql_y.y"
    {
	    yyval.field = yyvsp[-1].field;
	    abort_id = NULL;
	}
    break;

  case 12:
#line 240 "ql_y.y"
    {
	    yyval.field = NULL;
	    abort_id = yyvsp[0].str;
	}
    break;

  case 13:
#line 247 "ql_y.y"
    {
	    yyval.field = NULL;
	}
    break;

  case 14:
#line 251 "ql_y.y"
    {
	    yyval.field = yyvsp[-1].field;
	    yyvsp[-1].field->next = yyvsp[0].field;
	}
    break;

  case 15:
#line 259 "ql_y.y"
    {
	    TAG *walk;

	    yyval.field = yyvsp[0].field;
	    yyval.field->name_list = yyvsp[-2].nlist;
	    yyval.field->brk = yyvsp[-4].num;
	    yyval.field->id = yyvsp[-3].str;
	    if (yyval.field->var_len == -2) {
		if (*yyval.field->id == '_') yyerror("var-len field must be named");
	    }
	    else if (*yyval.field->id == '_' && !yyval.field->value)
		    yyerror("unnamed fields must have value");
	    if (*yyval.field->id == '_' && yyval.field->value && yyval.field->value->type == vt_case)
		for (walk = yyval.field->value->tags; walk; walk = walk->next)
		    if (walk->more)
			yyerror("value list only allowed in named case "
			  "selections");
	    if (*yyval.field->id != '_' && yyval.field->value && yyval.field->value->type == vt_multi)
		yyerror("multi selectors must be unnamed");
	}
    break;

  case 16:
#line 282 "ql_y.y"
    {
	    yyval.num = 0;
	}
    break;

  case 17:
#line 286 "ql_y.y"
    {
	    yyval.num = 1;
	}
    break;

  case 18:
#line 293 "ql_y.y"
    {
	    yyval.field = alloc_t(FIELD);
	    yyval.field->size = yyvsp[-1].num;
	    yyval.field->var_len = -2; /* hack */
	    if (yyvsp[-1].num & 7) yyerror("var-len field must have integral size");
	    yyval.field->pos = 0;
	    yyval.field->flush = 1;
	    yyval.field->value = NULL;
	    yyval.field->structure = NULL;
	    yyval.field->next = NULL;
	}
    break;

  case 19:
#line 305 "ql_y.y"
    {
	    yyval.field = alloc_t(FIELD);
	    yyval.field->size = yyvsp[-4].num;
	    yyval.field->var_len = -1;
	    yyval.field->pos = yyvsp[-3].num;
	    yyval.field->flush = !yyvsp[-2].num;
	    if (yyval.field->pos == -1) {
		if (yyval.field->size & 7)
		    yyerror("position required for small fields");
		else yyval.field->pos = 0;
	    }
	    yyval.field->value = yyvsp[0].value;
	    yyval.field->structure = NULL;
	    yyval.field->next = NULL;
	}
    break;

  case 20:
#line 323 "ql_y.y"
    {
	    yyval.num = -1;
	}
    break;

  case 21:
#line 327 "ql_y.y"
    {
	    yyval.num = yyvsp[0].num-1;
	    if (yyval.num < 0 || yyval.num > 7) yyerror("invalid position");
	}
    break;

  case 22:
#line 335 "ql_y.y"
    {
	    char *end;

	    yyval.num = strtoul(yyvsp[0].str,&end,10);
	    if (*end) yyerror("no a decimal number");
	}
    break;

  case 23:
#line 344 "ql_y.y"
    {
	    yyval.num = 0;
	}
    break;

  case 24:
#line 348 "ql_y.y"
    {
	    if (strcmp(yyvsp[0].str,"more")) yyerror("\"more\" expected");
	    yyval.num = 1;
	}
    break;

  case 25:
#line 355 "ql_y.y"
    {
	    yyval.value = NULL;
	}
    break;

  case 26:
#line 359 "ql_y.y"
    {
	    yyval.value = yyvsp[0].value;
	}
    break;

  case 27:
#line 366 "ql_y.y"
    {
	    yyval.value = alloc_t(VALUE);
	    yyval.value->type = vt_id;
	    yyval.value->id = yyvsp[0].str;
	}
    break;

  case 28:
#line 372 "ql_y.y"
    {
	    yyval.value = alloc_t(VALUE);
	    yyval.value->type = vt_case;
	    yyval.value->id = NULL;
	    yyval.value->tags = yyvsp[-1].tag;
	}
    break;

  case 29:
#line 379 "ql_y.y"
    {
	    yyval.value = alloc_t(VALUE);
	    yyval.value->type = vt_multi;
	    yyval.value->tags = yyvsp[-1].tag;
	}
    break;

  case 30:
#line 385 "ql_y.y"
    {
	    yyval.value = alloc_t(VALUE);
	    yyval.value->type = vt_length;
	    yyval.value->recovery = yyvsp[-2].str;
	    yyval.value->block = yyvsp[0].field;
	    yyval.value->abort_id = abort_id;
	}
    break;

  case 31:
#line 395 "ql_y.y"
    {
	    yyval.str = NULL;
	}
    break;

  case 32:
#line 399 "ql_y.y"
    {
	    yyval.str = yyvsp[0].str;
	}
    break;

  case 33:
#line 405 "ql_y.y"
    {
	    yyval.nlist = NULL;
	}
    break;

  case 34:
#line 409 "ql_y.y"
    {
	    yyval.nlist = get_name_list(yyvsp[0].str);
	}
    break;

  case 35:
#line 415 "ql_y.y"
    {
	    yyval.tag = NULL;
	}
    break;

  case 36:
#line 419 "ql_y.y"
    {
	    yyval.tag = alloc_t(TAG);
	    yyval.tag->deflt = 1;
	    if (yyvsp[-2].str) {
		yyval.tag->id = yyvsp[-3].str;
		yyval.tag->value = yyvsp[-2].str;
	    }
	    else {
		yyval.tag->id = NULL;
		yyval.tag->value = yyvsp[-3].str;
	    }
	    yyval.tag->more = yyvsp[-1].list;
	    yyval.tag->block = yyvsp[0].field;
	    yyval.tag->next = NULL;
	    yyval.tag->abort_id = abort_id;
	}
    break;

  case 37:
#line 436 "ql_y.y"
    {
	    yyval.tag = alloc_t(TAG);
	    yyval.tag->abort_id = abort_id;
	}
    break;

  case 38:
#line 441 "ql_y.y"
    {
	    yyval.tag = yyvsp[-1].tag;
	    yyval.tag->deflt = 0;
	    if (yyvsp[-4].str) {
		yyval.tag->id = yyvsp[-5].str;
		yyval.tag->value = yyvsp[-4].str;
	    }
	    else {
		yyval.tag->id = NULL;
		yyval.tag->value = yyvsp[-5].str;
	    }
	    yyval.tag->more = yyvsp[-3].list;
	    yyval.tag->block = yyvsp[-2].field;
	    yyval.tag->next = yyvsp[0].tag;
	}
    break;

  case 39:
#line 459 "ql_y.y"
    {
	    yyval.tag = NULL;
	}
    break;

  case 40:
#line 463 "ql_y.y"
    {
	    yyval.tag = alloc_t(TAG);
	    yyval.tag->deflt = 1;
	    if (yyvsp[-2].str) {
		yyval.tag->id = yyvsp[-3].str;
		yyval.tag->value = yyvsp[-2].str;
	    }
	    else {
		yyval.tag->id = NULL;
		yyval.tag->value = yyvsp[-3].str;
	    }
	    yyval.tag->more = yyvsp[-1].list;
	    yyval.tag->block = yyvsp[0].field;
	    yyval.tag->next = NULL;
	}
    break;

  case 41:
#line 479 "ql_y.y"
    {
	    yyval.tag = alloc_t(TAG);
	    yyval.tag->abort_id = abort_id;
	}
    break;

  case 42:
#line 484 "ql_y.y"
    {
	    yyval.tag = yyvsp[-1].tag;
	    yyval.tag->deflt = 0;
	    if (yyvsp[-4].str) {
		yyval.tag->id = yyvsp[-5].str;
		yyval.tag->value = yyvsp[-4].str;
	    }
	    else {
		yyval.tag->id = NULL;
		yyval.tag->value = yyvsp[-5].str;
	    }
	    yyval.tag->more = yyvsp[-3].list;
	    yyval.tag->block = yyvsp[-2].field;
	    yyval.tag->next = yyvsp[0].tag;
	}
    break;

  case 43:
#line 502 "ql_y.y"
    {
	    yyval.str = NULL;
	}
    break;

  case 44:
#line 506 "ql_y.y"
    {
	    yyval.str = yyvsp[0].str;
	}
    break;

  case 45:
#line 512 "ql_y.y"
    {
	    yyval.list = NULL;
	}
    break;

  case 46:
#line 516 "ql_y.y"
    {
	    yyval.list = alloc_t(VALUE_LIST);
	    yyval.list->value = yyvsp[-1].str;
	    yyval.list->next = yyvsp[0].list;
	}
    break;


    }

/* Line 1010 of yacc.c.  */
#line 1643 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}



