/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_VOID = 258,
    TOK_INT = 259,
    TOK_BOOL = 260,
    TOK_TRUE = 261,
    TOK_FALSE = 262,
    TOK_IF = 263,
    TOK_WHILE = 264,
    TOK_FOR = 265,
    TOK_DO = 266,
    TOK_PRINT = 267,
    TOK_SEMICOL = 268,
    TOK_COMMA = 269,
    TOK_LPAR = 270,
    TOK_RPAR = 271,
    TOK_LACC = 272,
    TOK_RACC = 273,
    TOK_THEN = 274,
    TOK_ELSE = 275,
    TOK_AFFECT = 276,
    TOK_OR = 277,
    TOK_AND = 278,
    TOK_BOR = 279,
    TOK_BXOR = 280,
    TOK_BAND = 281,
    TOK_EQ = 282,
    TOK_NE = 283,
    TOK_GT = 284,
    TOK_LT = 285,
    TOK_GE = 286,
    TOK_LE = 287,
    TOK_SRL = 288,
    TOK_SRA = 289,
    TOK_SLL = 290,
    TOK_PLUS = 291,
    TOK_MINUS = 292,
    TOK_MUL = 293,
    TOK_DIV = 294,
    TOK_MOD = 295,
    TOK_UMINUS = 296,
    TOK_NOT = 297,
    TOK_BNOT = 298,
    TOK_INTVAL = 299,
    TOK_IDENT = 300,
    TOK_STRING = 301
  };
#endif
/* Tokens.  */
#define TOK_VOID 258
#define TOK_INT 259
#define TOK_BOOL 260
#define TOK_TRUE 261
#define TOK_FALSE 262
#define TOK_IF 263
#define TOK_WHILE 264
#define TOK_FOR 265
#define TOK_DO 266
#define TOK_PRINT 267
#define TOK_SEMICOL 268
#define TOK_COMMA 269
#define TOK_LPAR 270
#define TOK_RPAR 271
#define TOK_LACC 272
#define TOK_RACC 273
#define TOK_THEN 274
#define TOK_ELSE 275
#define TOK_AFFECT 276
#define TOK_OR 277
#define TOK_AND 278
#define TOK_BOR 279
#define TOK_BXOR 280
#define TOK_BAND 281
#define TOK_EQ 282
#define TOK_NE 283
#define TOK_GT 284
#define TOK_LT 285
#define TOK_GE 286
#define TOK_LE 287
#define TOK_SRL 288
#define TOK_SRA 289
#define TOK_SLL 290
#define TOK_PLUS 291
#define TOK_MINUS 292
#define TOK_MUL 293
#define TOK_DIV 294
#define TOK_MOD 295
#define TOK_UMINUS 296
#define TOK_NOT 297
#define TOK_BNOT 298
#define TOK_INTVAL 299
#define TOK_IDENT 300
#define TOK_STRING 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "grammar.y"

    int32_t intval;
    char * strval;
    node_t ptr;

#line 155 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (node_t * program_root);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
