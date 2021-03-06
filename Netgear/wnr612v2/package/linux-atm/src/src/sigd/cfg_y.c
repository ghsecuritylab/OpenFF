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
     TOK_LEVEL = 258,
     TOK_DEBUG = 259,
     TOK_INFO = 260,
     TOK_WARN = 261,
     TOK_ERROR = 262,
     TOK_FATAL = 263,
     TOK_SIG = 264,
     TOK_UNI30 = 265,
     TOK_UNI31 = 266,
     TOK_UNI40 = 267,
     TOK_Q2963_1 = 268,
     TOK_SAAL = 269,
     TOK_VC = 270,
     TOK_IO = 271,
     TOK_MODE = 272,
     TOK_USER = 273,
     TOK_NET = 274,
     TOK_SWITCH = 275,
     TOK_VPCI = 276,
     TOK_ITF = 277,
     TOK_PCR = 278,
     TOK_TRACE = 279,
     TOK_POLICY = 280,
     TOK_ALLOW = 281,
     TOK_REJECT = 282,
     TOK_ENTITY = 283,
     TOK_DEFAULT = 284,
     TOK_NUMBER = 285,
     TOK_MAX_RATE = 286,
     TOK_DUMP_DIR = 287,
     TOK_LOGFILE = 288,
     TOK_QOS = 289,
     TOK_FROM = 290,
     TOK_TO = 291,
     TOK_ROUTE = 292,
     TOK_PVC = 293
   };
#endif
#define TOK_LEVEL 258
#define TOK_DEBUG 259
#define TOK_INFO 260
#define TOK_WARN 261
#define TOK_ERROR 262
#define TOK_FATAL 263
#define TOK_SIG 264
#define TOK_UNI30 265
#define TOK_UNI31 266
#define TOK_UNI40 267
#define TOK_Q2963_1 268
#define TOK_SAAL 269
#define TOK_VC 270
#define TOK_IO 271
#define TOK_MODE 272
#define TOK_USER 273
#define TOK_NET 274
#define TOK_SWITCH 275
#define TOK_VPCI 276
#define TOK_ITF 277
#define TOK_PCR 278
#define TOK_TRACE 279
#define TOK_POLICY 280
#define TOK_ALLOW 281
#define TOK_REJECT 282
#define TOK_ENTITY 283
#define TOK_DEFAULT 284
#define TOK_NUMBER 285
#define TOK_MAX_RATE 286
#define TOK_DUMP_DIR 287
#define TOK_LOGFILE 288
#define TOK_QOS 289
#define TOK_FROM 290
#define TOK_TO 291
#define TOK_ROUTE 292
#define TOK_PVC 293




/* Copy the first part of user declarations.  */
#line 1 "cfg_y.y"

/* cfg.y - configuration language */

/* Written 1995-1999 by Werner Almesberger, EPFL-LRC/ICA */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "atm.h"
#include "atmd.h"

#include "proto.h"
#include "io.h"
#include "trace.h"
#include "policy.h"

extern void yywarn(const char *s);
extern void yyerror(const char *s);

static RULE *rule;
static SIG_ENTITY *curr_sig = &_entity;


static int hex2num(char digit)
{
    if (isdigit(digit)) return digit-'0';
    if (islower(digit)) return toupper(digit)-'A'+10;
    return digit-'A'+10;
}


static void put_address(char *address)
{
    char *mask;

    mask = strchr(address,'/');
    if (mask) *mask++ = 0;
    if (text2atm(address,(struct sockaddr *) &rule->addr,sizeof(rule->addr),
      T2A_SVC | T2A_WILDCARD | T2A_NAME | T2A_LOCAL) < 0) {
	yyerror("invalid address");
	return;
    }
    if (!mask) rule->mask = -1;
    else rule->mask = strtol(mask,NULL,10);
    add_rule(rule);
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
#line 56 "cfg_y.y"
typedef union YYSTYPE {
    int num;
    char *str;
    struct sockaddr_atmpvc pvc;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 213 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 225 "y.tab.c"

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
#define YYFINAL  53
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   108

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  31
/* YYNRULES -- Number of rules. */
#define YYNRULES  79
/* YYNRULES -- Number of states. */
#define YYNSTATES  117

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,    39,     2,    40,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     6,     7,    10,    11,    14,    17,    20,
      23,    26,    29,    32,    33,    38,    39,    43,    44,    47,
      52,    55,    57,    59,    61,    63,    65,    69,    70,    73,
      75,    79,    80,    83,    85,    89,    90,    93,    95,    99,
     100,   103,   105,   109,   110,   113,   116,   121,   123,   125,
     127,   129,   131,   134,   137,   140,   143,   146,   148,   150,
     153,   155,   157,   160,   161,   163,   165,   167,   169,   171,
     173,   175,   177,   179,   182,   183,   187,   189,   191,   193
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      42,     0,    -1,    43,    44,    -1,    -1,    45,    43,    -1,
      -1,    46,    44,    -1,     3,    66,    -1,     9,    51,    -1,
      14,    53,    -1,    16,    55,    -1,     4,    57,    -1,    25,
      59,    -1,    -1,    28,    38,    47,    48,    -1,    -1,    39,
      49,    40,    -1,    -1,    50,    49,    -1,    21,    30,    22,
      30,    -1,    17,    67,    -1,    34,    -1,    31,    -1,    37,
      -1,    29,    -1,    61,    -1,    39,    52,    40,    -1,    -1,
      61,    52,    -1,    62,    -1,    39,    54,    40,    -1,    -1,
      62,    54,    -1,    63,    -1,    39,    56,    40,    -1,    -1,
      63,    56,    -1,    64,    -1,    39,    58,    40,    -1,    -1,
      64,    58,    -1,    68,    -1,    39,    60,    40,    -1,    -1,
      68,    60,    -1,     3,    66,    -1,    21,    30,    22,    30,
      -1,    10,    -1,    11,    -1,    12,    -1,    13,    -1,    19,
      -1,    17,    67,    -1,     3,    66,    -1,     3,    66,    -1,
      15,    38,    -1,    23,    30,    -1,    34,    -1,    31,    -1,
       3,    66,    -1,    32,    -1,    33,    -1,    24,    65,    -1,
      -1,    30,    -1,     4,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,    18,    -1,    19,    -1,    20,    -1,     3,
      66,    -1,    -1,    70,    69,    71,    -1,    26,    -1,    27,
      -1,    35,    -1,    36,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    77,    77,    80,    81,    84,    85,   109,   113,   114,
     115,   116,   117,   122,   121,   151,   152,   155,   156,   160,
     164,   165,   169,   173,   187,   194,   195,   198,   199,   203,
     204,   207,   208,   212,   213,   216,   217,   221,   222,   225,
     226,   230,   231,   234,   235,   239,   245,   249,   258,   267,
     276,   285,   290,   294,   302,   306,   310,   315,   319,   326,
     330,   335,   339,   346,   349,   356,   360,   364,   368,   372,
     379,   383,   387,   394,   399,   398,   407,   411,   418,   423
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_LEVEL", "TOK_DEBUG", "TOK_INFO",
  "TOK_WARN", "TOK_ERROR", "TOK_FATAL", "TOK_SIG", "TOK_UNI30",
  "TOK_UNI31", "TOK_UNI40", "TOK_Q2963_1", "TOK_SAAL", "TOK_VC", "TOK_IO",
  "TOK_MODE", "TOK_USER", "TOK_NET", "TOK_SWITCH", "TOK_VPCI", "TOK_ITF",
  "TOK_PCR", "TOK_TRACE", "TOK_POLICY", "TOK_ALLOW", "TOK_REJECT",
  "TOK_ENTITY", "TOK_DEFAULT", "TOK_NUMBER", "TOK_MAX_RATE",
  "TOK_DUMP_DIR", "TOK_LOGFILE", "TOK_QOS", "TOK_FROM", "TOK_TO",
  "TOK_ROUTE", "TOK_PVC", "'{'", "'}'", "$accept", "all", "global",
  "local", "item", "entity", "@1", "opt_options", "options", "option",
  "sig", "sig_items", "saal", "saal_items", "io", "io_items", "debug",
  "debug_items", "policy", "policy_items", "sig_item", "saal_item",
  "io_item", "debug_item", "opt_trace_size", "level", "mode",
  "policy_item", "@2", "action", "direction", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   123,
     125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    45,    45,    45,
      45,    45,    45,    47,    46,    48,    48,    49,    49,    50,
      50,    50,    50,    50,    50,    51,    51,    52,    52,    53,
      53,    54,    54,    55,    55,    56,    56,    57,    57,    58,
      58,    59,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    62,    63,    63,    63,    63,    63,    64,
      64,    64,    64,    65,    65,    66,    66,    66,    66,    66,
      67,    67,    67,    68,    69,    68,    70,    70,    71,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     0,     2,     0,     2,     2,     2,     2,
       2,     2,     2,     0,     4,     0,     3,     0,     2,     4,
       2,     1,     1,     1,     1,     1,     3,     0,     2,     1,
       3,     0,     2,     1,     3,     0,     2,     1,     3,     0,
       2,     1,     3,     0,     2,     2,     4,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     1,     1,     2,
       1,     1,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     3,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     0,     0,     0,     0,     0,     0,     5,     3,
      65,    66,    67,    68,    69,     7,     0,    63,    60,    61,
      39,    11,    37,     0,    47,    48,    49,    50,     0,    51,
       0,    27,     8,    25,     0,    31,     9,    29,     0,     0,
       0,    58,    57,    35,    10,    33,     0,    76,    77,    43,
      12,    41,    74,     1,     0,     2,     5,     4,    59,    64,
      62,     0,    39,    45,    70,    71,    72,    52,     0,     0,
      27,    53,     0,    31,    54,    55,    56,     0,    35,    73,
       0,    43,     0,    13,     6,    38,    40,     0,    26,    28,
      30,    32,    34,    36,    42,    44,    78,    79,    75,    15,
      46,    17,    14,     0,     0,    24,    22,    21,    23,     0,
      17,    20,     0,    16,    18,     0,    19
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     7,     8,    55,     9,    56,    99,   102,   109,   110,
      32,    69,    36,    72,    44,    77,    21,    61,    50,    80,
      70,    73,    78,    62,    60,    15,    67,    81,    82,    52,
      98
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -31
static const yysigned_char yypact[] =
{
      53,    69,     1,    -3,    -1,    -2,     0,    11,   -16,    53,
     -31,   -31,   -31,   -31,   -31,   -31,    69,   -15,   -31,   -31,
      28,   -31,   -31,    69,   -31,   -31,   -31,   -31,    46,   -31,
     -13,    80,   -31,   -31,    69,    16,   -31,   -31,    69,   -18,
      -6,   -31,   -31,    20,   -31,   -31,    69,   -31,   -31,    19,
     -31,   -31,   -31,   -31,   -10,   -31,   -16,   -31,   -31,   -31,
     -31,     2,    28,   -31,   -31,   -31,   -31,   -31,     8,     4,
      80,   -31,     7,    16,   -31,   -31,   -31,     9,    20,   -31,
      10,    19,   -30,   -31,   -31,   -31,   -31,    23,   -31,   -31,
     -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,    31,
     -31,    51,   -31,    46,    49,   -31,   -31,   -31,   -31,    15,
      51,   -31,    36,   -31,   -31,    54,   -31
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -31,   -31,    72,    30,   -31,   -31,   -31,   -31,   -23,   -31,
     -31,    24,   -31,    22,   -31,    18,   -31,    27,   -31,    17,
      97,    98,    99,   101,   -31,    25,     3,   102,   -31,   -31,
     -31
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      23,    38,    34,    46,    16,    96,    97,    24,    25,    26,
      27,    53,    54,    39,    28,    59,    29,    68,    30,    34,
      75,    40,    46,    38,    76,    17,    47,    48,    83,    41,
      87,    16,    42,    18,    19,    39,    31,    43,    35,    49,
      20,    58,    85,    40,    88,    47,    48,    90,    63,    92,
      94,    41,    17,   100,    42,   113,     1,     2,   115,    71,
      18,    19,     3,    74,    64,    65,    66,     4,   103,     5,
     101,    79,   104,    10,    11,    12,    13,    14,     6,   112,
     105,    57,   106,    23,   116,   107,    84,   114,   108,    86,
      24,    25,    26,    27,    89,    91,    93,    28,    95,    29,
      33,    30,    37,    22,    45,     0,   111,     0,    51
};

static const yysigned_char yycheck[] =
{
       3,     3,     3,     3,     3,    35,    36,    10,    11,    12,
      13,     0,    28,    15,    17,    30,    19,    30,    21,     3,
      38,    23,     3,     3,    30,    24,    26,    27,    38,    31,
      22,     3,    34,    32,    33,    15,    39,    39,    39,    39,
      39,    16,    40,    23,    40,    26,    27,    40,    23,    40,
      40,    31,    24,    30,    34,    40,     3,     4,    22,    34,
      32,    33,     9,    38,    18,    19,    20,    14,    17,    16,
      39,    46,    21,     4,     5,     6,     7,     8,    25,    30,
      29,     9,    31,     3,    30,    34,    56,   110,    37,    62,
      10,    11,    12,    13,    70,    73,    78,    17,    81,    19,
       3,    21,     4,     2,     5,    -1,   103,    -1,     6
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     9,    14,    16,    25,    42,    43,    45,
       4,     5,     6,     7,     8,    66,     3,    24,    32,    33,
      39,    57,    64,     3,    10,    11,    12,    13,    17,    19,
      21,    39,    51,    61,     3,    39,    53,    62,     3,    15,
      23,    31,    34,    39,    55,    63,     3,    26,    27,    39,
      59,    68,    70,     0,    28,    44,    46,    43,    66,    30,
      65,    58,    64,    66,    18,    19,    20,    67,    30,    52,
      61,    66,    54,    62,    66,    38,    30,    56,    63,    66,
      60,    68,    69,    38,    44,    40,    58,    22,    40,    52,
      40,    54,    40,    56,    40,    60,    35,    36,    71,    47,
      30,    39,    48,    17,    21,    29,    31,    34,    37,    49,
      50,    67,    30,    40,    49,    22,    30
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
        case 6:
#line 86 "cfg_y.y"
    {
	    if (!curr_sig->uni)
		curr_sig->uni =
#if defined(UNI30) || defined(DYNAMIC_UNI)
		  S_UNI30
#endif
#ifdef UNI31
		  S_UNI31
#ifdef ALLOW_UNI30
		  | S_UNI30
#endif
#endif
#ifdef UNI40
		  S_UNI40
#ifdef Q2963_1
		  | S_Q2963_1
#endif
#endif
		  ;
	}
    break;

  case 7:
#line 110 "cfg_y.y"
    {
	    set_verbosity(NULL,yyvsp[0].num);
	}
    break;

  case 13:
#line 122 "cfg_y.y"
    {
	    SIG_ENTITY *sig,**walk;

	    if (atmpvc_addr_in_use(_entity.signaling_pvc))
		yyerror("can't use  io vc  and  entity ...  in the same "
		  "configuration");
	    if (entities == &_entity) entities = NULL;
	    for (sig = entities; sig; sig = sig->next)
		if (atm_equal((struct sockaddr *) &sig->signaling_pvc,
		  (struct sockaddr *) &yyvsp[0].pvc,0,0)) {
		    const char *err;
		    asprintf(&err,"duplicate PVC address %d.%d.%d",S_PVC(sig));
		    if(err) {
		    	yyerror(err);
			free(err);
	  	    }
		    else
		    	yyerror("duplicate PVC address");
		}
	    curr_sig = alloc_t(SIG_ENTITY);
	    *curr_sig = _entity;
	    curr_sig->signaling_pvc = yyvsp[0].pvc;
	    curr_sig->next = NULL;
	    for (walk = &entities; *walk; walk = &(*walk)->next);
	    *walk = curr_sig;
	}
    break;

  case 19:
#line 161 "cfg_y.y"
    {
	    enter_vpci(curr_sig,yyvsp[-2].num,yyvsp[0].num);
	}
    break;

  case 21:
#line 166 "cfg_y.y"
    {
	    curr_sig->sig_qos = yyvsp[0].str;
	}
    break;

  case 22:
#line 170 "cfg_y.y"
    {
	    curr_sig->max_rate = yyvsp[0].num;
	}
    break;

  case 23:
#line 174 "cfg_y.y"
    {
	     struct sockaddr_atmsvc addr;
	     char *mask;

	    mask = strchr(yyvsp[0].str,'/');
	    if (mask) *mask++ = 0;
	    if (text2atm(yyvsp[0].str,(struct sockaddr *) &addr,sizeof(addr),
	    T2A_SVC | T2A_WILDCARD | T2A_NAME | T2A_LOCAL) < 0) {
		yyerror("invalid address");
		return 0;
	    }
	    add_route(curr_sig,&addr,mask ? strtol(mask,NULL,10) : INT_MAX);
	}
    break;

  case 24:
#line 188 "cfg_y.y"
    {
	    add_route(curr_sig,NULL,0);
	}
    break;

  case 45:
#line 240 "cfg_y.y"
    {
	    set_verbosity("UNI",yyvsp[0].num);
	    set_verbosity("KERNEL",yyvsp[0].num);
	    set_verbosity("SAP",yyvsp[0].num);
	}
    break;

  case 46:
#line 246 "cfg_y.y"
    {
	    enter_vpci(curr_sig,yyvsp[-2].num,yyvsp[0].num);
	}
    break;

  case 47:
#line 250 "cfg_y.y"
    {
#if defined(UNI30) || defined(ALLOW_UNI30) || defined(DYNAMIC_UNI)
	    if (curr_sig->uni & ~S_UNI31) yyerror("UNI mode is already set");
	    curr_sig->uni |= S_UNI30;
#else
	    yyerror("Sorry, not supported yet");
#endif
	}
    break;

  case 48:
#line 259 "cfg_y.y"
    {
#if defined(UNI31) || defined(ALLOW_UNI30) || defined(DYNAMIC_UNI)
	    if (curr_sig->uni & ~S_UNI30) yyerror("UNI mode is already set");
	    curr_sig->uni |= S_UNI31;
#else
	    yyerror("Sorry, not supported yet");
#endif
	}
    break;

  case 49:
#line 268 "cfg_y.y"
    {
#if defined(UNI40) || defined(DYNAMIC_UNI)
	    if (curr_sig->uni) yyerror("UNI mode is already set");
	    curr_sig->uni = S_UNI40;
#else
	    yyerror("Sorry, not supported yet");
#endif
	}
    break;

  case 50:
#line 277 "cfg_y.y"
    {
#if defined(Q2963_1) || defined(DYNAMIC_UNI)
	    if (!(curr_sig->uni & S_UNI40)) yyerror("Incompatible UNI mode");
	    curr_sig->uni |= S_Q2963_1;
#else
	    yyerror("Sorry, not supported yet");
#endif
	}
    break;

  case 51:
#line 286 "cfg_y.y"
    {
	    yywarn("sig net  is obsolete, please use  sig mode net  instead");
	    curr_sig->mode = sm_net;
	}
    break;

  case 53:
#line 295 "cfg_y.y"
    {
	    set_verbosity("SSCF",yyvsp[0].num);
	    set_verbosity("SSCOP",yyvsp[0].num);
	}
    break;

  case 54:
#line 303 "cfg_y.y"
    {
	    set_verbosity("IO",yyvsp[0].num);
	}
    break;

  case 55:
#line 307 "cfg_y.y"
    {
	    curr_sig->signaling_pvc = yyvsp[0].pvc;
	}
    break;

  case 56:
#line 311 "cfg_y.y"
    {
	    yywarn("io pcr  is obsolete, please use  io qos  instead");
	    curr_sig->sig_pcr = yyvsp[0].num;
	}
    break;

  case 57:
#line 316 "cfg_y.y"
    {
	    curr_sig->sig_qos = yyvsp[0].str;
	}
    break;

  case 58:
#line 320 "cfg_y.y"
    {
	    curr_sig->max_rate = yyvsp[0].num;
	}
    break;

  case 59:
#line 327 "cfg_y.y"
    {
	    set_verbosity(NULL,yyvsp[0].num);
	}
    break;

  case 60:
#line 331 "cfg_y.y"
    {
	    dump_dir = yyvsp[0].str;
	    if (!trace_size) trace_size = DEFAULT_TRACE_SIZE;
	}
    break;

  case 61:
#line 336 "cfg_y.y"
    {
	    set_logfile(yyvsp[0].str);
	}
    break;

  case 62:
#line 340 "cfg_y.y"
    {
	    trace_size = yyvsp[0].num;
	}
    break;

  case 63:
#line 346 "cfg_y.y"
    {
	    yyval.num = DEFAULT_TRACE_SIZE;
	}
    break;

  case 64:
#line 350 "cfg_y.y"
    {
	    yyval.num = yyvsp[0].num;
	}
    break;

  case 65:
#line 357 "cfg_y.y"
    {
	    yyval.num = DIAG_DEBUG;
	}
    break;

  case 66:
#line 361 "cfg_y.y"
    {
	    yyval.num = DIAG_INFO;
	}
    break;

  case 67:
#line 365 "cfg_y.y"
    {
	    yyval.num = DIAG_WARN;
	}
    break;

  case 68:
#line 369 "cfg_y.y"
    {
	    yyval.num = DIAG_ERROR;
	}
    break;

  case 69:
#line 373 "cfg_y.y"
    {
	    yyval.num = DIAG_FATAL;
	}
    break;

  case 70:
#line 380 "cfg_y.y"
    {
	    curr_sig->mode = sm_user;
	}
    break;

  case 71:
#line 384 "cfg_y.y"
    {
	    curr_sig->mode = sm_net;
	}
    break;

  case 72:
#line 388 "cfg_y.y"
    {
	    curr_sig->mode = sm_switch;
	}
    break;

  case 73:
#line 395 "cfg_y.y"
    {
	    set_verbosity("POLICY",yyvsp[0].num);
	}
    break;

  case 74:
#line 399 "cfg_y.y"
    {
	    rule = alloc_t(RULE);
	    rule->type = yyvsp[0].num;
	}
    break;

  case 76:
#line 408 "cfg_y.y"
    {
	    yyval.num = ACL_ALLOW;
	}
    break;

  case 77:
#line 412 "cfg_y.y"
    {
	    yyval.num = ACL_REJECT;
	}
    break;

  case 78:
#line 419 "cfg_y.y"
    {
	    rule->type |= ACL_IN;
	    put_address(yyvsp[0].str);
	}
    break;

  case 79:
#line 424 "cfg_y.y"
    {
	    rule->type |= ACL_OUT;
	    put_address(yyvsp[0].str);
	}
    break;


    }

/* Line 1010 of yacc.c.  */
#line 1585 "y.tab.c"

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



