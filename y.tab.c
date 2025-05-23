/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "miniC.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboles.h"
#include "genererDot.h"

void ouvrir_graphe();
void fermer_graphe();
int generer_dot_node(Node *node);

 

#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_GREEN "\033[32m"
#define RESET_COLOR "\033[0m"
#define DEBUG 1

#define EMIT_WARNING(fmt, ...) do { \
    fprintf(stderr, COLOR_PURPLE "[Warning] " RESET_COLOR fmt "\n", ##__VA_ARGS__); \
    fprintf(stderr, "          at line %d, near '%s'\n", yylineno, yytext); \
} while(0) // while(0) pour que les différents printf soient dans le même bloc

#define EMIT_ERROR(fmt, ...) do { \
    fprintf(stderr, COLOR_RED "[Error] " RESET_COLOR fmt "\n", ##__VA_ARGS__); \
    fprintf(stderr, "        at line %d, near '%s'\n", yylineno, yytext); \
    exit(1); \
} while(0)

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

int yylex(void);       
void yyerror(char *s);
void error(char *s);
void warn(char *s);
void yylex_destroy(void);

FILE *file;


NodeList *liste_fonctions = NULL; // liste globale des fonctions A SUPP
Node *current_function = NULL; // fonction courante
int mode_affectation = 0; // mode déclaration ou pas



#line 122 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CONSTANTE = 258,               /* CONSTANTE  */
    IDENTIFICATEUR = 259,          /* IDENTIFICATEUR  */
    VOID = 260,                    /* VOID  */
    INT = 261,                     /* INT  */
    FOR = 262,                     /* FOR  */
    WHILE = 263,                   /* WHILE  */
    IF = 264,                      /* IF  */
    ELSE = 265,                    /* ELSE  */
    SWITCH = 266,                  /* SWITCH  */
    CASE = 267,                    /* CASE  */
    DEFAULT = 268,                 /* DEFAULT  */
    BREAK = 269,                   /* BREAK  */
    RETURN = 270,                  /* RETURN  */
    PLUS = 271,                    /* PLUS  */
    MOINS = 272,                   /* MOINS  */
    MUL = 273,                     /* MUL  */
    DIV = 274,                     /* DIV  */
    LSHIFT = 275,                  /* LSHIFT  */
    RSHIFT = 276,                  /* RSHIFT  */
    BAND = 277,                    /* BAND  */
    BOR = 278,                     /* BOR  */
    LAND = 279,                    /* LAND  */
    LOR = 280,                     /* LOR  */
    LT = 281,                      /* LT  */
    GT = 282,                      /* GT  */
    GEQ = 283,                     /* GEQ  */
    LEQ = 284,                     /* LEQ  */
    EQ = 285,                      /* EQ  */
    NEQ = 286,                     /* NEQ  */
    NOT = 287,                     /* NOT  */
    EXTERN = 288,                  /* EXTERN  */
    THEN = 289,                    /* THEN  */
    MOINSUNAIRE = 290,             /* MOINSUNAIRE  */
    REL = 291                      /* REL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define CONSTANTE 258
#define IDENTIFICATEUR 259
#define VOID 260
#define INT 261
#define FOR 262
#define WHILE 263
#define IF 264
#define ELSE 265
#define SWITCH 266
#define CASE 267
#define DEFAULT 268
#define BREAK 269
#define RETURN 270
#define PLUS 271
#define MOINS 272
#define MUL 273
#define DIV 274
#define LSHIFT 275
#define RSHIFT 276
#define BAND 277
#define BOR 278
#define LAND 279
#define LOR 280
#define LT 281
#define GT 282
#define GEQ 283
#define LEQ 284
#define EQ 285
#define NEQ 286
#define NOT 287
#define EXTERN 288
#define THEN 289
#define MOINSUNAIRE 290
#define REL 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 52 "miniC.y"

    int entier;
    char *chaine;
	type_t type;

	struct Node *node;
	struct NodeList *node_list;
	struct NodeList **node_table;

#line 257 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CONSTANTE = 3,                  /* CONSTANTE  */
  YYSYMBOL_IDENTIFICATEUR = 4,             /* IDENTIFICATEUR  */
  YYSYMBOL_VOID = 5,                       /* VOID  */
  YYSYMBOL_INT = 6,                        /* INT  */
  YYSYMBOL_FOR = 7,                        /* FOR  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_SWITCH = 11,                    /* SWITCH  */
  YYSYMBOL_CASE = 12,                      /* CASE  */
  YYSYMBOL_DEFAULT = 13,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 14,                     /* BREAK  */
  YYSYMBOL_RETURN = 15,                    /* RETURN  */
  YYSYMBOL_PLUS = 16,                      /* PLUS  */
  YYSYMBOL_MOINS = 17,                     /* MOINS  */
  YYSYMBOL_MUL = 18,                       /* MUL  */
  YYSYMBOL_DIV = 19,                       /* DIV  */
  YYSYMBOL_LSHIFT = 20,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 21,                    /* RSHIFT  */
  YYSYMBOL_BAND = 22,                      /* BAND  */
  YYSYMBOL_BOR = 23,                       /* BOR  */
  YYSYMBOL_LAND = 24,                      /* LAND  */
  YYSYMBOL_LOR = 25,                       /* LOR  */
  YYSYMBOL_LT = 26,                        /* LT  */
  YYSYMBOL_GT = 27,                        /* GT  */
  YYSYMBOL_GEQ = 28,                       /* GEQ  */
  YYSYMBOL_LEQ = 29,                       /* LEQ  */
  YYSYMBOL_EQ = 30,                        /* EQ  */
  YYSYMBOL_NEQ = 31,                       /* NEQ  */
  YYSYMBOL_NOT = 32,                       /* NOT  */
  YYSYMBOL_EXTERN = 33,                    /* EXTERN  */
  YYSYMBOL_34_ = 34,                       /* '='  */
  YYSYMBOL_THEN = 35,                      /* THEN  */
  YYSYMBOL_MOINSUNAIRE = 36,               /* MOINSUNAIRE  */
  YYSYMBOL_REL = 37,                       /* REL  */
  YYSYMBOL_38_ = 38,                       /* ';'  */
  YYSYMBOL_39_ = 39,                       /* ','  */
  YYSYMBOL_40_ = 40,                       /* '['  */
  YYSYMBOL_41_ = 41,                       /* ']'  */
  YYSYMBOL_42_ = 42,                       /* '('  */
  YYSYMBOL_43_ = 43,                       /* ')'  */
  YYSYMBOL_44_ = 44,                       /* '{'  */
  YYSYMBOL_45_ = 45,                       /* '}'  */
  YYSYMBOL_46_ = 46,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_programme = 48,                 /* programme  */
  YYSYMBOL_liste_declarations = 49,        /* liste_declarations  */
  YYSYMBOL_liste_fonctions = 50,           /* liste_fonctions  */
  YYSYMBOL_declaration = 51,               /* declaration  */
  YYSYMBOL_liste_declarateurs = 52,        /* liste_declarateurs  */
  YYSYMBOL_declarateur = 53,               /* declarateur  */
  YYSYMBOL_ouverture_fonction = 54,        /* ouverture_fonction  */
  YYSYMBOL_fonction = 55,                  /* fonction  */
  YYSYMBOL_type = 56,                      /* type  */
  YYSYMBOL_liste_parms = 57,               /* liste_parms  */
  YYSYMBOL_parm = 58,                      /* parm  */
  YYSYMBOL_liste_instructions = 59,        /* liste_instructions  */
  YYSYMBOL_instruction = 60,               /* instruction  */
  YYSYMBOL_iteration = 61,                 /* iteration  */
  YYSYMBOL_selection = 62,                 /* selection  */
  YYSYMBOL_liste_cases_opt = 63,           /* liste_cases_opt  */
  YYSYMBOL_liste_cases = 64,               /* liste_cases  */
  YYSYMBOL_case = 65,                      /* case  */
  YYSYMBOL_default_opt = 66,               /* default_opt  */
  YYSYMBOL_saut = 67,                      /* saut  */
  YYSYMBOL_affectation = 68,               /* affectation  */
  YYSYMBOL_opn_bloc = 69,                  /* opn_bloc  */
  YYSYMBOL_close_bloc = 70,                /* close_bloc  */
  YYSYMBOL_bloc = 71,                      /* bloc  */
  YYSYMBOL_variable = 72,                  /* variable  */
  YYSYMBOL_dimension_utilisation = 73,     /* dimension_utilisation  */
  YYSYMBOL_expression = 74,                /* expression  */
  YYSYMBOL_liste_expressions = 75,         /* liste_expressions  */
  YYSYMBOL_condition = 76,                 /* condition  */
  YYSYMBOL_binary_rel = 77,                /* binary_rel  */
  YYSYMBOL_binary_comp = 78                /* binary_comp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      42,    43,     2,     2,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    38,
       2,    34,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    35,
      36,    37
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    97,    97,   159,   185,   191,   197,   205,   209,   222,
     227,   233,   241,   251,   267,   275,   290,   291,   296,   299,
     305,   308,   319,   344,   350,   353,   356,   359,   362,   365,
     369,   375,   382,   390,   395,   401,   411,   415,   423,   430,
     443,   454,   457,   467,   477,   482,   501,   507,   513,   525,
     542,   573,   578,   584,   591,   597,   600,   603,   607,   616,
     620,   623,   626,   629,   635,   639,   680,   685,   690,   697,
     700,   704,   707,   719,   720,   724,   725,   726,   727,   728,
     729
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CONSTANTE",
  "IDENTIFICATEUR", "VOID", "INT", "FOR", "WHILE", "IF", "ELSE", "SWITCH",
  "CASE", "DEFAULT", "BREAK", "RETURN", "PLUS", "MOINS", "MUL", "DIV",
  "LSHIFT", "RSHIFT", "BAND", "BOR", "LAND", "LOR", "LT", "GT", "GEQ",
  "LEQ", "EQ", "NEQ", "NOT", "EXTERN", "'='", "THEN", "MOINSUNAIRE", "REL",
  "';'", "','", "'['", "']'", "'('", "')'", "'{'", "'}'", "':'", "$accept",
  "programme", "liste_declarations", "liste_fonctions", "declaration",
  "liste_declarateurs", "declarateur", "ouverture_fonction", "fonction",
  "type", "liste_parms", "parm", "liste_instructions", "instruction",
  "iteration", "selection", "liste_cases_opt", "liste_cases", "case",
  "default_opt", "saut", "affectation", "opn_bloc", "close_bloc", "bloc",
  "variable", "dimension_utilisation", "expression", "liste_expressions",
  "condition", "binary_rel", "binary_comp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-98)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -98,     7,     3,   -98,   -98,   -98,   102,     3,   -98,   -98,
      16,    19,   -98,    39,    13,   115,    45,    46,    13,    90,
      31,   -98,    95,    98,    90,   100,    -2,   -98,   -98,    29,
      31,    31,   -98,   194,   -98,    45,    70,     6,   -98,    90,
     -98,    31,    31,    82,   -98,   141,    31,    31,    31,    31,
      31,    31,    31,    31,   -98,   -98,    88,   -98,   101,   160,
     254,    17,    31,   -98,    -6,    -6,   168,   168,    96,    96,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,    31,   -98,   186,
     102,   254,   -98,    95,    83,   109,   110,   132,   143,   117,
       2,   -98,   -98,   -98,   -98,   -98,   148,   -98,   -98,   159,
     202,   190,    21,    21,    31,   -98,   -98,   225,   -98,    31,
     -98,   155,   158,   159,   184,    21,   238,    22,    37,   149,
     -98,   254,    21,    21,   113,    50,   -98,   -98,   -98,   -98,
     -98,   -98,    31,   -98,   -98,   106,    21,   106,   153,    43,
      81,   -98,   254,   -98,   -98,   218,   217,   190,   -98,   106,
     227,   220,   217,   -98,   188,   -98,   189,   191,   193,   -98,
     106,   106,   106,   -98,   -98,   123,   123,   -98,   -98
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     1,    16,    17,     0,     2,     3,     6,
       0,     0,     5,     0,    11,     0,    10,     0,     0,    20,
       0,     7,     0,     0,    20,     0,     0,    18,    63,    49,
       0,     0,    64,     0,    11,     9,     0,     0,    21,     0,
      13,     0,    68,    50,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     8,    12,     0,    19,     0,     0,
      67,     0,     0,    53,    55,    56,    57,    58,    59,    60,
      61,    62,    15,    46,     4,    14,    52,     0,    65,     0,
      23,    66,    51,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    22,    26,    27,    28,     0,    48,    29,    64,
       0,     0,     0,     0,     0,    41,    42,     0,    24,     0,
      25,    49,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    45,     0,     0,     0,     0,    75,    76,    77,    78,
      79,    80,     0,    73,    74,    30,     0,    30,     0,     0,
       0,    71,    72,    32,    70,    33,     0,     0,    69,    30,
       0,     0,    36,    38,     0,    34,     0,     0,     0,    37,
      30,    30,    30,    35,    31,    44,    44,    39,    40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   -98,   162,   -98,   -98,   -98,   212,   -98,   232,    -4,
     226,   210,   -98,    38,   -98,   -98,   -98,   -98,    99,   -98,
     -19,   -97,   -98,   -98,   195,   -83,   -98,   -20,   -98,   -64,
     -98,   -98
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     7,     8,    15,    16,    58,     9,    10,
      26,    27,    84,    92,    93,    94,   151,   152,   153,   158,
      95,    96,    74,    97,    98,    32,    43,   100,    61,   117,
     136,   132
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      33,    99,    11,    13,   112,    28,    29,     3,     4,     5,
      44,    45,    48,    49,    50,    51,    52,    53,   113,    30,
      14,    59,    60,    17,    28,    29,    64,    65,    66,    67,
      68,    69,    70,    71,    28,    29,     6,    39,    30,   118,
     106,    40,    79,    18,    31,    39,   133,   134,    30,    56,
     154,   125,    99,   114,    99,    19,    77,    81,   139,   140,
      78,   133,   134,   115,   113,   135,    99,   133,   134,    41,
     107,    42,   144,    31,   133,   134,    83,    99,    99,    99,
     137,   147,   116,   116,   119,    23,    28,    29,    24,   121,
      85,    86,    87,   141,    88,   124,    25,    89,    90,    34,
      30,    36,   116,   116,    38,   133,   134,     4,     5,    28,
      29,    55,   142,    85,    86,    87,   116,    88,    52,    53,
      89,    90,    62,    30,   148,    31,    72,    73,    91,    46,
      47,    48,    49,    50,    51,    52,    53,    89,    90,   126,
     127,   128,   129,   130,   131,    73,   167,   168,    31,    20,
      73,   101,   102,    21,    22,   105,    63,    46,    47,    48,
      49,    50,    51,    52,    53,    46,    47,    48,    49,    50,
      51,    52,    53,   143,   103,   145,    46,    47,    48,    49,
      50,    51,    52,    53,    63,   104,   108,   155,    50,    51,
      52,    53,   138,   109,   111,    41,   122,   146,   164,   165,
     166,    76,    46,    47,    48,    49,    50,    51,    52,    53,
      46,    47,    48,    49,    50,    51,    52,    53,    46,    47,
      48,    49,    50,    51,    52,    53,   123,    82,   149,   150,
     156,   160,    54,   157,    35,   161,    80,   162,   163,    12,
     110,    46,    47,    48,    49,    50,    51,    52,    53,    57,
      37,   159,     0,    75,    46,    47,    48,    49,    50,    51,
      52,    53,     0,   120,   126,   127,   128,   129,   130,   131,
      46,    47,    48,    49,    50,    51,    52,    53
};

static const yytype_int16 yycheck[] =
{
      20,    84,     6,     7,   101,     3,     4,     0,     5,     6,
      30,    31,    18,    19,    20,    21,    22,    23,   101,    17,
       4,    41,    42,     4,     3,     4,    46,    47,    48,    49,
      50,    51,    52,    53,     3,     4,    33,    39,    17,   103,
      38,    43,    62,     4,    42,    39,    24,    25,    17,    43,
     147,   115,   135,    32,   137,    42,    39,    77,   122,   123,
      43,    24,    25,    42,   147,    43,   149,    24,    25,    40,
      90,    42,   136,    42,    24,    25,    80,   160,   161,   162,
      43,    38,   102,   103,   104,    40,     3,     4,    42,   109,
       7,     8,     9,    43,    11,   115,     6,    14,    15,     4,
      17,     3,   122,   123,     4,    24,    25,     5,     6,     3,
       4,    41,   132,     7,     8,     9,   136,    11,    22,    23,
      14,    15,    40,    17,    43,    42,    38,    44,    45,    16,
      17,    18,    19,    20,    21,    22,    23,    14,    15,    26,
      27,    28,    29,    30,    31,    44,   165,   166,    42,    34,
      44,    42,    42,    38,    39,    38,    43,    16,    17,    18,
      19,    20,    21,    22,    23,    16,    17,    18,    19,    20,
      21,    22,    23,   135,    42,   137,    16,    17,    18,    19,
      20,    21,    22,    23,    43,    42,    38,   149,    20,    21,
      22,    23,    43,    34,     4,    40,    38,    44,   160,   161,
     162,    41,    16,    17,    18,    19,    20,    21,    22,    23,
      16,    17,    18,    19,    20,    21,    22,    23,    16,    17,
      18,    19,    20,    21,    22,    23,    42,    41,    10,    12,
       3,    43,    38,    13,    22,    46,    74,    46,    45,     7,
      38,    16,    17,    18,    19,    20,    21,    22,    23,    39,
      24,   152,    -1,    58,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    38,    26,    27,    28,    29,    30,    31,
      16,    17,    18,    19,    20,    21,    22,    23
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    48,    49,     0,     5,     6,    33,    50,    51,    55,
      56,    56,    55,    56,     4,    52,    53,     4,     4,    42,
      34,    38,    39,    40,    42,     6,    57,    58,     3,     4,
      17,    42,    72,    74,     4,    53,     3,    57,     4,    39,
      43,    40,    42,    73,    74,    74,    16,    17,    18,    19,
      20,    21,    22,    23,    38,    41,    43,    58,    54,    74,
      74,    75,    40,    43,    74,    74,    74,    74,    74,    74,
      74,    74,    38,    44,    69,    71,    41,    39,    43,    74,
      49,    74,    41,    56,    59,     7,     8,     9,    11,    14,
      15,    45,    60,    61,    62,    67,    68,    70,    71,    72,
      74,    42,    42,    42,    42,    38,    38,    74,    38,    34,
      38,     4,    68,    72,    32,    42,    74,    76,    76,    74,
      38,    74,    38,    42,    74,    76,    26,    27,    28,    29,
      30,    31,    78,    24,    25,    43,    77,    43,    43,    76,
      76,    43,    74,    60,    76,    60,    44,    38,    43,    10,
      12,    63,    64,    65,    68,    60,     3,    13,    66,    65,
      43,    46,    46,    45,    60,    60,    60,    67,    67
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    51,    51,    52,
      52,    53,    53,    54,    55,    55,    56,    56,    57,    57,
      57,    58,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    61,    61,    62,    62,    62,    63,    64,    64,    65,
      66,    67,    67,    67,    67,    68,    69,    70,    71,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    75,    75,    75,    76,
      76,    76,    76,    77,    77,    78,    78,    78,    78,    78,
      78
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     2,     1,     3,     5,     3,
       1,     1,     4,     0,     7,     7,     1,     1,     1,     3,
       0,     2,     2,     0,     2,     2,     1,     1,     1,     1,
       0,     9,     5,     5,     7,     8,     1,     2,     1,     5,
       4,     2,     2,     3,     0,     3,     1,     1,     4,     1,
       2,     4,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     4,     3,     1,     0,     4,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programme: liste_declarations liste_fonctions  */
#line 97 "miniC.y"
                                                   {
			printf("Programme complet\n");
			ouvrir_graphe();	// ouverture du fichier dot 

			if (DEBUG) printf("Programme :\n");
			if (DEBUG) printf("├── Déclarations globales\n");
			NodeList *tmp;
			for (int i = 0; i < TAILLE; i++) {
				if ((yyvsp[-1].node_table)[i] != NULL) {
					tmp = (yyvsp[-1].node_table)[i];
					while (tmp != NULL) {
						// printf("│   ├── %s\n", tmp->node->symbole.nom);
						if (DEBUG) afficher_node2("│   ├──", tmp->node);
						// Génération du graphe pour chaque déclaration globale
                    	// generer_dot_node(tmp->node);

						tmp = tmp->suivant;
					}
				}
			}
			if (DEBUG) printf("└── Fonctions :\n");
			tmp = (yyvsp[0].node_list);
			

			while (tmp != NULL) {
				generer_dot_node(tmp->node);
				if (DEBUG) printf("    ├── %s\n", tmp->node->fonction.nom);
				if (DEBUG) printf("    │	│\n");
				if (DEBUG) printf("    │	├── Type : %s\n", tmp->node->fonction.type == ENTIER ? "int" : "void");
				NodeList *tmp2 = tmp->node->fonction.liste_parametres;
				if (DEBUG) printf("    │	├── Paramètres : (");
				while (tmp2 != NULL) {
					if (DEBUG) printf("%s, ", tmp2->node->parametre.nom);
					tmp2 = tmp2->suivant;
				}
				if (tmp->node->fonction.externe) {
					if (DEBUG) printf(") EXTERNE\n");
					tmp = tmp->suivant;

					continue;
				} 
				if (DEBUG) printf(")\n");
				if (DEBUG) afficher_node2("    │	", tmp->node->fonction.bloc);
				tmp = tmp->suivant;
				
			}
		
        fermer_graphe(); // fermeture fichier dot
        printf(">> Graphe DOT généré avec succès.\n");


		// free_liste(liste_fonctions); // on libère la liste de fonctions
		// free_list($1); // on libère la liste de déclarations
		// free_list($2); // on libère la liste de fonctions
		// free_all(); // on libère la table de symboles
		
		// free_table($1);
		// free_list($2);
		}
#line 1505 "y.tab.c"
    break;

  case 3: /* liste_declarations: liste_declarations declaration  */
#line 159 "miniC.y"
                                               {
			(yyval.node_table) = (yyvsp[-1].node_table);
			NodeList *tmp = (yyvsp[0].node_list);
			while (tmp != NULL) {
				int h = hash(tmp->node->symbole.nom);
				if ((yyval.node_table)[h] == NULL) { // si la ligne de la table est vide
					// on ajoute le noeud à la table de hachage
					(yyval.node_table)[h] = tmp;
					tmp = tmp->suivant;
					(yyval.node_table)[h]->suivant = NULL; // on met le suivant à NULL
					// pour pas ajouter toute la liste chaînée
				} else {
					NodeList *tmp2 = (yyval.node_table)[h];
					while (tmp2 != NULL) { // on verifie si la variable existe déjà
						if (strcmp(tmp2->node->symbole.nom, tmp->node->symbole.nom) == 0) {
							// yyerror("Variable already declared in this scope");
							EMIT_ERROR("Variable already declared in this scope");
						}
						tmp2 = tmp2->suivant;
					}
					tmp->suivant = (yyval.node_table)[h]; // on ajoute le noeud au début de la liste
					(yyval.node_table)[h] = tmp; // on met à jour la table de hachage
					tmp = tmp->suivant;
				}
			}
		}
#line 1536 "y.tab.c"
    break;

  case 4: /* liste_declarations: %empty  */
#line 185 "miniC.y"
                        {
			(yyval.node_table) = creer_node_table(); 
		}
#line 1544 "y.tab.c"
    break;

  case 5: /* liste_fonctions: liste_fonctions fonction  */
#line 191 "miniC.y"
                                         {
			if (append_node((yyvsp[-1].node_list), (yyvsp[0].node))) {
				EMIT_ERROR("Redeclaration de la fonction : %s", (yyvsp[0].node)->fonction.nom);
			}
			(yyval.node_list) = (yyvsp[-1].node_list);
		}
#line 1555 "y.tab.c"
    break;

  case 6: /* liste_fonctions: fonction  */
#line 197 "miniC.y"
                         {
			(yyval.node_list) = nouveau_node_list((yyvsp[0].node));
			// printf(COLOR_GREEN "1 : Node LIST %d\n" RESET_COLOR, $$->id);
				
		}
#line 1565 "y.tab.c"
    break;

  case 7: /* declaration: type liste_declarateurs ';'  */
#line 205 "miniC.y"
                                            {
			(yyval.node_list) = (yyvsp[-1].node_list);
		}
#line 1573 "y.tab.c"
    break;

  case 8: /* declaration: type liste_declarateurs '=' expression ';'  */
#line 209 "miniC.y"
                                                            {
			(yyval.node_list) = (yyvsp[-3].node_list);
			NodeList *tmp = (yyvsp[-3].node_list);
			while (tmp->suivant != NULL) { // on va a la derniere variable
				tmp = tmp->suivant;
			}
			tmp->node->symbole.valeur = (yyvsp[-1].node)->expression.valeur; // on affecte la valeur de l'expression à la variable
			tmp->node->symbole.evaluable = (yyvsp[-1].node)->expression.evaluable; // on met la variable comme évaluable
			tmp->node->symbole.isInitialized = 1; // on met la variable comme initialisée
		}
#line 1588 "y.tab.c"
    break;

  case 9: /* liste_declarateurs: liste_declarateurs ',' declarateur  */
#line 222 "miniC.y"
                                                   {
			// on ajoute declarateur au début de la liste
			// on verifie pas encore si la variable existe déjà
			append_node((yyvsp[-2].node_list), (yyvsp[0].node));
		}
#line 1598 "y.tab.c"
    break;

  case 10: /* liste_declarateurs: declarateur  */
#line 227 "miniC.y"
                            {
		(yyval.node_list) = nouveau_node_list((yyvsp[0].node));
	}
#line 1606 "y.tab.c"
    break;

  case 11: /* declarateur: IDENTIFICATEUR  */
#line 233 "miniC.y"
                       {
			// On crée un nouveau noeud pour la déclaration de variable
			// on verifie pas encore si la variable existe déjà
			(yyval.node) = nouveau_node(SYMBOLE);
        	(yyval.node)->symbole.nom = (yyvsp[0].chaine);

			ajouter_variable((yyval.node)); // on ajoute la variable à la table de symboles courante
		}
#line 1619 "y.tab.c"
    break;

  case 12: /* declarateur: declarateur '[' CONSTANTE ']'  */
#line 241 "miniC.y"
                                              {
			// on modifie le noeud de la déclaration de variable
			(yyval.node) = (yyvsp[-3].node);
			(yyval.node)->symbole.type = TABLEAU;
			(yyval.node)->symbole.dimension += 1; // on incremente la dimension du tableau
			(yyval.node)->symbole.isInitialized = 1; // on met initialisée parce que ce n'est pas encore géré
		}
#line 1631 "y.tab.c"
    break;

  case 13: /* ouverture_fonction: %empty  */
#line 251 "miniC.y"
        {
		// dans le cas d'une fonction reccursive il faut ajouter la fonction à la table de symboles
		// avant de l'initialiser, sinon on ne peut pas l'utiliser dans le corps de la fonction
		push_table(); // ouverture de bloc
		(yyval.node) = nouveau_node(FONCTION);
		(yyval.node)->fonction.nom = (yyvsp[(-2) - (0)].chaine);
		(yyval.node)->fonction.type = (yyvsp[(-4) - (0)].type);
		(yyval.node)->fonction.liste_parametres = (yyvsp[(-1) - (0)].node_list);
		(yyval.node)->fonction.bloc = NULL;
		(yyval.node)->fonction.externe = 0; // on met la fonction comme interne
		ajouter_fonction((yyval.node)); // on ajoute la fonction à la table de symboles courante
		current_function = (yyval.node); // on initialise la fonction courante pour verifier les types de return
	}
#line 1649 "y.tab.c"
    break;

  case 14: /* fonction: type IDENTIFICATEUR '(' liste_parms ')' ouverture_fonction bloc  */
#line 267 "miniC.y"
                                                                    {
			(yyval.node) = (yyvsp[-1].node);
			(yyval.node)->fonction.type = (yyvsp[-6].type);
			(yyval.node)->fonction.bloc = (yyvsp[0].node);
			append_node(liste_fonctions, (yyval.node));
			pop_table(); // fermeture de bloc
			current_function = NULL; // on remet la fonction courante à NULL
		}
#line 1662 "y.tab.c"
    break;

  case 15: /* fonction: EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';'  */
#line 275 "miniC.y"
                                                             {
			(yyval.node) = nouveau_node(FONCTION);
			(yyval.node)->fonction.nom = (yyvsp[-4].chaine);
			(yyval.node)->fonction.type = (yyvsp[-5].type);
			(yyval.node)->fonction.liste_parametres = (yyvsp[-2].node_list);
			(yyval.node)->fonction.bloc = NULL;
			(yyval.node)->fonction.externe = 1; // on met la fonction comme externe

			append_node(liste_fonctions, (yyval.node)); //! VERIFIER L'UTILITÉ
			ajouter_fonction((yyval.node)); // on ajoute la fonction à la table de symboles courante
		}
#line 1678 "y.tab.c"
    break;

  case 16: /* type: VOID  */
#line 290 "miniC.y"
                     { (yyval.type) = VOID_TYPE; }
#line 1684 "y.tab.c"
    break;

  case 17: /* type: INT  */
#line 291 "miniC.y"
             { (yyval.type) = ENTIER; }
#line 1690 "y.tab.c"
    break;

  case 18: /* liste_parms: parm  */
#line 296 "miniC.y"
                        {
			(yyval.node_list) = nouveau_node_list((yyvsp[0].node));
		}
#line 1698 "y.tab.c"
    break;

  case 19: /* liste_parms: liste_parms ',' parm  */
#line 299 "miniC.y"
                                        {
			(yyval.node_list) = (yyvsp[-2].node_list);
			if (append_node((yyval.node_list), (yyvsp[0].node))) {
				EMIT_ERROR("Paramètre déjà déclaré dans cette fonction : %s", (yyvsp[0].node)->parametre.nom);
			}
		}
#line 1709 "y.tab.c"
    break;

  case 20: /* liste_parms: %empty  */
#line 305 "miniC.y"
                       { (yyval.node_list) = NULL; }
#line 1715 "y.tab.c"
    break;

  case 21: /* parm: INT IDENTIFICATEUR  */
#line 308 "miniC.y"
                         {
	(yyval.node) = nouveau_node(PARAMETRE);
	(yyval.node)->parametre.type = ENTIER;
	(yyval.node)->parametre.nom = (yyvsp[0].chaine);

	ajouter_parametre((yyval.node)); // on ajoute le paramètre à la table de symboles courante
}
#line 1727 "y.tab.c"
    break;

  case 22: /* liste_instructions: liste_instructions instruction  */
#line 319 "miniC.y"
                                               {
			// on ajoute instruction a la fin de la liste
			// on verifie pas encore si la variable existe déjà
			if ((yyvsp[-1].node_list) == NULL) {
				(yyval.node_list) = nouveau_node_list((yyvsp[0].node));				
			} else {
				// on ajoute instruction au début de la liste
				// on verifie pas encore si la variable existe déjà
				(yyval.node_list) = (yyvsp[-1].node_list);
				NodeList *nouv = nouveau_node_list((yyvsp[0].node));
				nouv->suivant = NULL; // pour pas faire une boucle
				
				if ((yyval.node_list)->suivant == NULL) {
					(yyval.node_list)->suivant = nouv;
					(yyval.node_list)->precedent = nouv;
					nouv->precedent = (yyval.node_list);
				} else {
					nouv->precedent = (yyval.node_list)->precedent;
					(yyval.node_list)->precedent->suivant = nouv;
					(yyval.node_list)->precedent = nouv;
					(yyval.node_list)->precedent = nouv;

				}
			}
		}
#line 1757 "y.tab.c"
    break;

  case 23: /* liste_instructions: %empty  */
#line 344 "miniC.y"
                          {
			(yyval.node_list) = NULL; 
		}
#line 1765 "y.tab.c"
    break;

  case 24: /* instruction: affectation ';'  */
#line 350 "miniC.y"
                                         { 
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1773 "y.tab.c"
    break;

  case 25: /* instruction: expression ';'  */
#line 353 "miniC.y"
                                   {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1781 "y.tab.c"
    break;

  case 26: /* instruction: iteration  */
#line 356 "miniC.y"
                                                 {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1789 "y.tab.c"
    break;

  case 27: /* instruction: selection  */
#line 359 "miniC.y"
                                         {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1797 "y.tab.c"
    break;

  case 28: /* instruction: saut  */
#line 362 "miniC.y"
                                                 {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1805 "y.tab.c"
    break;

  case 29: /* instruction: bloc  */
#line 365 "miniC.y"
                                                 {
		(yyval.node) = (yyvsp[0].node);
	}
#line 1813 "y.tab.c"
    break;

  case 30: /* instruction: %empty  */
#line 369 "miniC.y"
                       { (yyval.node) = NULL; }
#line 1819 "y.tab.c"
    break;

  case 31: /* iteration: FOR '(' affectation ';' condition ';' affectation ')' instruction  */
#line 375 "miniC.y"
                                                                                   {
			(yyval.node) = nouveau_node(FOR_NODE);
			(yyval.node)->for_node.init = (yyvsp[-6].node);
			(yyval.node)->for_node.condition = (yyvsp[-4].node);
			(yyval.node)->for_node.incr = (yyvsp[-2].node);
			(yyval.node)->for_node.instruction = (yyvsp[0].node);
		}
#line 1831 "y.tab.c"
    break;

  case 32: /* iteration: WHILE '(' condition ')' instruction  */
#line 382 "miniC.y"
                                                        {
			(yyval.node) = nouveau_node(WHILE_NODE);
			(yyval.node)->while_node.condition = (yyvsp[-2].node);
			(yyval.node)->while_node.instruction = (yyvsp[0].node);
	}
#line 1841 "y.tab.c"
    break;

  case 33: /* selection: IF '(' condition ')' instruction  */
#line 390 "miniC.y"
                                                                    {
			(yyval.node) = nouveau_node(IF_NODE);
			(yyval.node)->if_node.instruction = (yyvsp[0].node);
			(yyval.node)->if_node.condition = (yyvsp[-2].node);
		}
#line 1851 "y.tab.c"
    break;

  case 34: /* selection: IF '(' condition ')' instruction ELSE instruction  */
#line 395 "miniC.y"
                                                                        {
			(yyval.node) = nouveau_node(IF_ELSE_NODE);
			(yyval.node)->if_else_node.instruction = (yyvsp[-2].node);
			(yyval.node)->if_else_node.condition = (yyvsp[-4].node);
			(yyval.node)->if_else_node.instruction_else = (yyvsp[0].node);
		}
#line 1862 "y.tab.c"
    break;

  case 35: /* selection: SWITCH '(' expression ')' '{' liste_cases_opt default_opt '}'  */
#line 401 "miniC.y"
                                                                              {
        	(yyval.node) = nouveau_node(SWITCH_NODE);
        	(yyval.node)->switch_node.expression = (yyvsp[-5].node);
        	(yyval.node)->switch_node.liste_cases = (yyvsp[-2].node_list);
        	(yyval.node)->switch_node.default_case = (yyvsp[-1].node);
    }
#line 1873 "y.tab.c"
    break;

  case 36: /* liste_cases_opt: liste_cases  */
#line 411 "miniC.y"
                { (yyval.node_list) = (yyvsp[0].node_list); }
#line 1879 "y.tab.c"
    break;

  case 37: /* liste_cases: liste_cases case  */
#line 415 "miniC.y"
                     {
        int valCase = (yyvsp[0].node)->case_node.constante->expression.valeur;
        if (chercher_case((yyvsp[-1].node_list), valCase)) { // On teste pour qu'il n' ait pas deux cases identiques
            EMIT_ERROR("Erreur : case %d déjà existante\n", valCase);    
        }
        append_node((yyvsp[-1].node_list), (yyvsp[0].node));
        (yyval.node_list) = (yyvsp[-1].node_list);
    }
#line 1892 "y.tab.c"
    break;

  case 38: /* liste_cases: case  */
#line 423 "miniC.y"
           {
        (yyval.node_list) = nouveau_node_list((yyvsp[0].node));
    }
#line 1900 "y.tab.c"
    break;

  case 39: /* case: CASE CONSTANTE ':' instruction saut  */
#line 430 "miniC.y"
                                        {
        (yyval.node) = nouveau_node(CASE_NODE);
        Node *cst = nouveau_node(EXPRESSION);
        cst->expression.type = EXPRESSION_CONSTANTE;
        cst->expression.valeur = (yyvsp[-3].entier);
        cst->expression.evaluable = 1;
        (yyval.node)->case_node.constante = cst;
        (yyval.node)->case_node.instruction = (yyvsp[-1].node);
        (yyval.node)->case_node.saut = (yyvsp[0].node);
    }
#line 1915 "y.tab.c"
    break;

  case 40: /* default_opt: DEFAULT ':' instruction saut  */
#line 443 "miniC.y"
                                 {
        (yyval.node) = nouveau_node(DEFAULT_NODE);
        (yyval.node)->default_node.instruction = (yyvsp[-1].node);
        (yyval.node)->default_node.saut = (yyvsp[0].node);	
    }
#line 1925 "y.tab.c"
    break;

  case 41: /* saut: BREAK ';'  */
#line 454 "miniC.y"
                                {
			(yyval.node) = nouveau_node(BREAK_NODE);
		}
#line 1933 "y.tab.c"
    break;

  case 42: /* saut: RETURN ';'  */
#line 457 "miniC.y"
                                {
			(yyval.node) = nouveau_node(RETURN_NODE);
			(yyval.node)->return_node.expression = NULL;
			if (current_function == NULL) {
				yyerror("RETURN sans valeur dans une fonction void");
			}
			if (current_function->fonction.type == ENTIER) {
				yyerror("RETURN sans valeur dans une fonction int");
			}
	}
#line 1948 "y.tab.c"
    break;

  case 43: /* saut: RETURN expression ';'  */
#line 467 "miniC.y"
                                        {
			(yyval.node) = nouveau_node(RETURN_NODE);
			(yyval.node)->return_node.expression = (yyvsp[-1].node);
			if (current_function == NULL) {
				yyerror("RETURN avec valeur dans une fonction void");
			}
			if (current_function->fonction.type == VOID_TYPE) {
				yyerror("RETURN avec valeur dans une fonction void");
			}
	}
#line 1963 "y.tab.c"
    break;

  case 44: /* saut: %empty  */
#line 477 "miniC.y"
                     { (yyval.node) = NULL; }
#line 1969 "y.tab.c"
    break;

  case 45: /* affectation: variable '=' expression  */
#line 482 "miniC.y"
                                        { // on véifie si expression contient une variable non initialisée
			if (verifier_initialisation_expression((yyvsp[0].node))){
				EMIT_WARNING("Variable utilisée sans être initialisée : %s", (yyvsp[0].node)->symbole.nom);
			}
			(yyvsp[-2].node)->symbole.isInitialized = 1; // on met la variable comme initialisée
			if ((yyvsp[0].node)->expression.evaluable == 1) {
				(yyvsp[-2].node)->symbole.valeur = (yyvsp[0].node)->expression.valeur; // on affecte la valeur de l'expression à la variable
				(yyvsp[-2].node)->symbole.evaluable = 1; // on met la variable comme évaluable
			} else {
				(yyvsp[-2].node)->symbole.evaluable = 0; // on met la variable comme non évaluable
			}
			(yyval.node) = nouveau_node(AFFECTATION);
			(yyval.node)->affectation.variable = (yyvsp[-2].node);
			(yyval.node)->affectation.expression = (yyvsp[0].node);
		}
#line 1989 "y.tab.c"
    break;

  case 46: /* opn_bloc: '{'  */
#line 501 "miniC.y"
                    {
			push_table(); // ouverture de bloc
		}
#line 1997 "y.tab.c"
    break;

  case 47: /* close_bloc: '}'  */
#line 507 "miniC.y"
                    {
			pop_table(); // fermeture de bloc
		}
#line 2005 "y.tab.c"
    break;

  case 48: /* bloc: opn_bloc liste_declarations liste_instructions close_bloc  */
#line 513 "miniC.y"
                                                                  {
			(yyval.node) = nouveau_node(BLOC);
			(yyval.node)->bloc.table_declarations = (yyvsp[-2].node_table);
			(yyval.node)->bloc.liste_instructions = (yyvsp[-1].node_list);
			// printf(COLOR_GREEN);
			// printf("Bloc :\n");
			// printf(RESET_COLOR);
        }
#line 2018 "y.tab.c"
    break;

  case 49: /* variable: IDENTIFICATEUR  */
#line 525 "miniC.y"
                               {
			/* On vérifie si la variable existe dans la table de symboles */
			Node *result = chercher_symbole((yyvsp[0].chaine));
			if (result == NULL) {
				size_t alloc_len = strlen("Variable utilisée mais jamais déclarée :") + strlen((yyvsp[0].chaine)) + 10; // Ajoutez une marge de sécurité // try
				char *s = malloc(alloc_len);
				if (s == NULL) {
					fprintf(stderr, "Erreur : allocation mémoire échouée\n");
					exit(EXIT_FAILURE);
				}
				EMIT_ERROR("Variable utilisée mais jamais déclarée : %s", (yyvsp[0].chaine));
			}
			 if (result->type == FONCTION) {
				EMIT_ERROR("Fonction utilisée comme variable");
			}
			(yyval.node) = result;
		}
#line 2040 "y.tab.c"
    break;

  case 50: /* variable: IDENTIFICATEUR dimension_utilisation  */
#line 542 "miniC.y"
                                                     {
		// on verifie si la variable existe
		int cpt = 0;
		Node *result = chercher_symbole((yyvsp[-1].chaine));
		if (result == NULL) {
			EMIT_ERROR("Variable utilisée mais jamais déclarée : %s", (yyvsp[-1].chaine));
		}
		if (result->type == FONCTION) {
			EMIT_ERROR("Fonction utilisée comme variable : %s", (yyvsp[-1].chaine));
		}
		if (result->symbole.type != TABLEAU) {
			EMIT_ERROR("Variable int utilisée comme tableau : %s", (yyvsp[-1].chaine));
		}
		NodeList *tmp = (yyvsp[0].node_list);
		while (tmp != NULL) {
			cpt++;
			tmp = tmp->suivant;
		}
		if (cpt != result->symbole.dimension) {
			EMIT_ERROR("Nombre de dimensions incorrect : %s", (yyvsp[-1].chaine));
		}
		(yyval.node) = nouveau_node(ACCES_TABLEAU);
		(yyval.node)->acces_tableau.variable = result;
		(yyval.node)->acces_tableau.liste_expressions = (yyvsp[0].node_list);
		


	}
#line 2073 "y.tab.c"
    break;

  case 51: /* dimension_utilisation: dimension_utilisation '[' expression ']'  */
#line 573 "miniC.y"
                                                         {
			(yyval.node_list) = (yyvsp[-3].node_list);
			// on ajoute l'expression à la liste d'expressions
			append_node((yyval.node_list), (yyvsp[-1].node));
		}
#line 2083 "y.tab.c"
    break;

  case 52: /* dimension_utilisation: '[' expression ']'  */
#line 578 "miniC.y"
                                   {
			(yyval.node_list) = nouveau_node_list((yyvsp[-1].node));
		}
#line 2091 "y.tab.c"
    break;

  case 53: /* expression: '(' expression ')'  */
#line 584 "miniC.y"
                                                                                { 
			(yyval.node) = nouveau_node(EXPRESSION);
			(yyval.node)->expression.type = EXPRESSION_PARENTHESE;
			(yyval.node)->expression.expression = (yyvsp[-1].node);
			(yyval.node)->expression.evaluable = (yyvsp[-1].node)->expression.evaluable;
			(yyval.node)->expression.valeur = (yyvsp[-1].node)->expression.valeur;
		}
#line 2103 "y.tab.c"
    break;

  case 54: /* expression: MOINS expression  */
#line 591 "miniC.y"
                                                                { 
			(yyval.node) = nouveau_node(EXPRESSION);
			(yyval.node)->expression.type = EXPRESSION_MOINS_UNAIRE;
			(yyval.node)->expression.expression = (yyvsp[0].node);
			(yyval.node)->expression.evaluable = (yyvsp[0].node)->expression.evaluable;
		}
#line 2114 "y.tab.c"
    break;

  case 55: /* expression: expression PLUS expression  */
#line 597 "miniC.y"
                                                                        { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "+");
		}
#line 2122 "y.tab.c"
    break;

  case 56: /* expression: expression MOINS expression  */
#line 600 "miniC.y"
                                                                { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "-");
		}
#line 2130 "y.tab.c"
    break;

  case 57: /* expression: expression MUL expression  */
#line 603 "miniC.y"
                                                                { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "*");

		}
#line 2139 "y.tab.c"
    break;

  case 58: /* expression: expression DIV expression  */
#line 607 "miniC.y"
                                                                { 
			int res;
			int eval;
			evaluer_expression((yyvsp[0].node), &res, &eval);
			if (eval && res == 0) {
				EMIT_WARNING("Division par zéro");
			}
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "/");
		}
#line 2153 "y.tab.c"
    break;

  case 59: /* expression: expression LSHIFT expression  */
#line 616 "miniC.y"
                                                                { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "<<");
			
		}
#line 2162 "y.tab.c"
    break;

  case 60: /* expression: expression RSHIFT expression  */
#line 620 "miniC.y"
                                                                { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), ">>");
		}
#line 2170 "y.tab.c"
    break;

  case 61: /* expression: expression BAND expression  */
#line 623 "miniC.y"
                                                                        { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "&");
		}
#line 2178 "y.tab.c"
    break;

  case 62: /* expression: expression BOR expression  */
#line 626 "miniC.y"
                                                                        { 
			(yyval.node) = construire_expr_binaire((yyvsp[-2].node), (yyvsp[0].node), "|");
		}
#line 2186 "y.tab.c"
    break;

  case 63: /* expression: CONSTANTE  */
#line 629 "miniC.y"
                                                                        { 
			(yyval.node) = nouveau_node(EXPRESSION);
			(yyval.node)->expression.type = EXPRESSION_CONSTANTE;
			(yyval.node)->expression.valeur = (yyvsp[0].entier);
			(yyval.node)->expression.evaluable = 1;
		}
#line 2197 "y.tab.c"
    break;

  case 64: /* expression: variable  */
#line 635 "miniC.y"
                                                                                                { 
			(yyval.node) = (yyvsp[0].node);

		}
#line 2206 "y.tab.c"
    break;

  case 65: /* expression: IDENTIFICATEUR '(' liste_expressions ')'  */
#line 639 "miniC.y"
                                                                { //appel de fonction 
			// on verifie si la fonction existe
			// on verifie si le nombre d'arguments est correct
			// si la fonction est recursive 
			Node *result = chercher_fonction((yyvsp[-3].chaine));
			if (result == NULL) {
				char *s = malloc( strlen("Fonction non déclarée :") + strlen((yyvsp[-3].chaine)) + 1);
				sprintf(s, "Fonction non déclarée : %s", (yyvsp[-3].chaine));
				error(s); // -> kill
			}
			if (result->type != FONCTION) {
				yyerror("Identificateur utilisé comme fonction");
			}
			if (result->type == FONCTION) {
				// on verifie si le nombre d'arguments est correct
				NodeList *tmp = (yyvsp[-1].node_list);
				NodeList *tmp2 = result->fonction.liste_parametres;
				int i = 0;
				while (tmp != NULL && tmp2 != NULL) {
					tmp = tmp->suivant;
					tmp2 = tmp2->suivant;
					i++;
				}
				if (tmp != NULL || tmp2 != NULL) {
					yyerror("Nombre d'arguments incorrect");
				}
				NodeList *indices = (yyvsp[-1].node_list); // try
            	while (indices) {
                	if (verifier_initialisation_expression(indices->node)) {
						EMIT_WARNING("Variable utilisée sans être initialisée : %s", indices->node->symbole.nom);
					}
                	indices = indices->suivant;
            	}
			}
			(yyval.node) = nouveau_node(APPEL_FONCTION);
			(yyval.node)->appel_fonction.nom = (yyvsp[-3].chaine);
			(yyval.node)->appel_fonction.liste_expressions = (yyvsp[-1].node_list);
		}
#line 2249 "y.tab.c"
    break;

  case 66: /* liste_expressions: liste_expressions ',' expression  */
#line 680 "miniC.y"
                                                 { 
			// printf("Liste d'expressions \n");
			(yyval.node_list) = (yyvsp[-2].node_list);
			append_node((yyval.node_list), (yyvsp[0].node));
		}
#line 2259 "y.tab.c"
    break;

  case 67: /* liste_expressions: expression  */
#line 685 "miniC.y"
                           { 
			// printf("Expression unique \n");
			(yyval.node_list) = nouveau_node_list((yyvsp[0].node));
			// printf(COLOR_GREEN "6 : Node LIST %d\n" RESET_COLOR, $$->id);
	}
#line 2269 "y.tab.c"
    break;

  case 68: /* liste_expressions: %empty  */
#line 690 "miniC.y"
                              { 
			// printf("Liste d'expressions vide\n");
			(yyval.node_list) = NULL; 
	}
#line 2278 "y.tab.c"
    break;

  case 69: /* condition: NOT '(' condition ')'  */
#line 697 "miniC.y"
                                      { 
			printf("Condition NOT\n"); 
		}
#line 2286 "y.tab.c"
    break;

  case 70: /* condition: condition binary_rel condition  */
#line 700 "miniC.y"
                                                         {
		printf("Condition relationnelle\n");
		//JSP CE QUE C'EST
	}
#line 2295 "y.tab.c"
    break;

  case 71: /* condition: '(' condition ')'  */
#line 704 "miniC.y"
                                   {
		printf("Condition\n"); 
	}
#line 2303 "y.tab.c"
    break;

  case 72: /* condition: expression binary_comp expression  */
#line 707 "miniC.y"
                                                   { 
			// printf("Condition binaire\n");
			(yyval.node) = nouveau_node(CONDITION_BINAIRE);
			(yyval.node)->condition_binaire.gauche = (yyvsp[-2].node);
			(yyval.node)->condition_binaire.droite = (yyvsp[0].node);
			(yyval.node)->condition_binaire.operateur = (yyvsp[-1].chaine);
	}
#line 2315 "y.tab.c"
    break;

  case 75: /* binary_comp: LT  */
#line 724 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2321 "y.tab.c"
    break;

  case 76: /* binary_comp: GT  */
#line 725 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2327 "y.tab.c"
    break;

  case 77: /* binary_comp: GEQ  */
#line 726 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2333 "y.tab.c"
    break;

  case 78: /* binary_comp: LEQ  */
#line 727 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2339 "y.tab.c"
    break;

  case 79: /* binary_comp: EQ  */
#line 728 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2345 "y.tab.c"
    break;

  case 80: /* binary_comp: NEQ  */
#line 729 "miniC.y"
                        { (yyval.chaine) = strdup(yytext); }
#line 2351 "y.tab.c"
    break;


#line 2355 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 731 "miniC.y"


void warn(char *s) {
	fprintf(stderr, COLOR_PURPLE);
	fprintf(stderr, "Warning: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Warning at line %d\n", yylineno);
	fprintf(stderr, "Warning near: %s\n", yytext);
}

void yyerror(char *s) {
	fprintf(stderr, COLOR_RED);
	fprintf(stderr, "Error: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error at line %d\n", yylineno);
	fprintf(stderr, "Error near: %s\n", yytext);
	exit(1);
}

void error(char *s) {
	/*comme yyerror mais avec un free*/
	fprintf(stderr, COLOR_RED);
	fprintf(stderr, "Error: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error at line %d\n", yylineno);
	fprintf(stderr, "Error near: %s\n", yytext);
	free(s);
	exit(1);
}

int main(int argc, char **argv) {
	if (argc > 1) {
		file = fopen(argv[1], "r");
		if (file == NULL) {
			fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
			exit(1);
		}
		yyin = file;
	} else {
		yyin = stdin;
	}
    push_table(); // init table globale

    yyparse();

    pop_table(); // nettoyage
	/* free_all(); // nettoyage */
	yylex_destroy();
	if (file != NULL) {
		fclose(file);
	}
	
    return 0;
}



/*
Node *result = chercher_variable($1);
if (result == NULL) {
	char *s = malloc( strlen("Variable utilisée mais jamais déclarée :") + strlen($1) + 1);
	sprintf(s, "Variable utilisée mais jamais déclarée : %s", $1);
	error(s); // -> kill
}
if (result->symbole.isInitialized == 0) {
	char *s = malloc( strlen("Variable utilisée mais jamais initialisée :") + strlen($1) + 1);
	sprintf(s, "Variable utilisée mais jamais initialisée : %s", $1);
	printf("------WARNING : Variable utilisée mais jamais initialisée : %s\n", $1);
	// WARNING -------------- A FAIRE
	// warning(s); // -> warning
}


*/
