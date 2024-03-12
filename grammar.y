%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "common.h"
#include "miniccutils.h"
#include "passe_1.h"
#include "passe_2.h"



/* Global variables */
extern bool stop_after_syntax;
extern bool stop_after_verif;
extern char * outfile;


/* prototypes */
int yylex(void);
extern int yylineno;

void yyerror(node_t * program_root, char * s);
void analyse_tree(node_t root);
node_t make_node(node_nature nature, int nops, ...);
/* A completer */


%}

%parse-param { node_t * program_root }

%union {
    int32_t intval;
    char * strval;
    node_t ptr;
};


/* Definir les token ici avec leur associativite, dans le bon ordre */
/* A completer */
%token TOK_VOID TOK_INT TOK_BOOL TOK_TRUE TOK_FALSE TOK_IF TOK_WHILE TOK_FOR TOK_DO TOK_PRINT TOK_SEMICOL
%token TOK_COMMA TOK_LPAR TOK_RPAR TOK_LACC TOK_RACC

%nonassoc TOK_ELSE
%nonassoc TOK_THEN

%right TOK_AFFECT

%left TOK_OR
%left TOK_AND
%left TOK_BOR
%left TOK_BXOR
%left TOK_BAND
%nonassoc TOK_EQ TOK_NE
%nonassoc TOK_GT TOK_LT TOK_GE TOK_LE
%nonassoc TOK_SRL TOK_SRA TOK_SLL

%left TOK_PLUS TOK_MINUS
%left TOK_MULT TOK_DIV TOK_MOD

%nonassoc TOK_UMINUS TOK_NOT TOK_BNOT




%token <intval> TOK_INTVAL;
%token <strval> TOK_IDENT TOK_STRING;

%type <ptr> program listdecl listdeclnonnull vardecl ident type listtypedecl decl maindecl
%type <ptr> listinst listinstnonnull inst block expr listparamprint paramprint 


%%

/* Completer les regles et la creation de l'arbre */
program:
        listdeclnonnull maindecl
        {
            $$ = make_node(NODE_PROGRAM, 2, $1, $2);
            *program_root = $$;
        }
        | maindecl
        {
            $$ = make_node(NODE_PROGRAM, 2, NULL, $1);
            *program_root = $$;
        }
        ;

listdecl: 
        listdeclnonnull
        { $$ = NULL; }
        | 
        { $$ = NULL; }
        ;

listdeclnonnull: 
        vardecl
        { $$ = NULL; }
        | listdeclnonnull vardecl
        { $$ = NULL; }
            
        ;

vardecl: 
        type listtypedecl TOK_SEMICOL
            { $$ = NULL; }
        
            ;

type:
    TOK_INT
        { $$ = NULL; }
        | TOK_BOOL
        { $$ = NULL; }
        | TOK_VOID
        { $$ = NULL; }

        ;

listtypedecl: 
        decl
            { $$ = $1; }   
            | listtypedecl TOK_COMMA decl
            { $$ = make_node(NODE_LIST, 2, $1, $3); }
            ;

decl:
    ident
        { $$ = NULL; }
        | ident TOK_AFFECT expr 
        { $$ = NULL; }
        ;

maindecl:
        type ident TOK_LPAR TOK_RPAR block
            { $$ = NULL; }
        ;

listinst: listinstnonnull
        { $$ = NULL; }
        | 
        { $$ = NULL; }
            ;

listinstnonnull: inst 
        { $$ = NULL; }
        | listinstnonnull inst 
        { $$ = NULL; }    
            ;

inst: 
    expr TOK_SEMICOL
        { $$ = NULL; }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst TOK_ELSE inst
        { $$ = make_node(NODE_IF,2,$1,$3  ); }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst %prec TOK_THEN
        { $$ = NULL; }
        | TOK_WHILE TOK_LPAR expr TOK_RPAR inst
        { $$ = NULL; }
        | TOK_FOR TOK_LPAR expr TOK_SEMICOL expr TOK_SEMICOL expr TOK_RPAR inst
        { $$ = NULL; }
        | TOK_DO inst TOK_WHILE TOK_LPAR expr TOK_RPAR TOK_SEMICOL
        { $$ = NULL; }
        | block
        { $$ = NULL; }
        | TOK_SEMICOL
        { $$ = NULL; }
        | TOK_PRINT TOK_LPAR listparamprint TOK_RPAR TOK_SEMICOL
        { $$ = NULL; }
        ;

block:
    TOK_LACC listdecl listinst TOK_RACC
            { $$ = NULL; }
            ;

expr: expr TOK_MULT expr 
            { $$ = make_node(NODE_MUL, 2, $1, $3); }
        | expr TOK_DIV expr
            { $$ = make_node(NODE_DIV, 2, $1, $3) ;}
        | expr TOK_PLUS expr
            { $$ = make_node(NODE_PLUS, 2, $1, $3) ;}
        | expr TOK_MINUS expr
            { $$ = make_node(NODE_MINUS, 2, $1, $3) ; }
        | expr TOK_MOD expr
            { $$ = make_node(NODE_MOD, 2, $1, $3) ; }
        | expr TOK_LT expr
            { $$ = make_node(NODE_LT, 2, $1, $3) ; }
        | expr TOK_GT expr
            { $$ = make_node(NODE_GT, 2, $1, $3) ; }
        | TOK_MINUS expr %prec TOK_UMINUS
            { $$ = make_node(NODE_MINUS, 1, $2); }
        | expr TOK_GE expr
            { $$ = make_node(NODE_GE, 2, $1, $3); }
        | expr TOK_LE expr
            { $$ = make_node(NODE_LE, 2, $1, $3); }
        | expr TOK_EQ expr
            { $$ = make_node(NODE_EQ, 2, $1, $3); }
        | expr TOK_NE expr
            { $$ = make_node(NODE_NE, 2, $1, $3); }    
        | expr TOK_AND expr
            { $$ = make_node(NODE_AND, 2, $1, $3); }
        | expr TOK_OR expr
            { $$ = make_node(NODE_OR, 2, $1, $3); }
        | expr TOK_BAND expr
            { $$ = make_node(NODE_BAND, 2, $1, $3); }
        | expr TOK_BOR expr
            { $$=make_node(NODE_BOR, 2, $1, $3) ; }
        | expr TOK_BXOR expr
            { $$ = make_node(NODE_BXOR, 2, $1, $3) ; }
        | expr TOK_SRL expr
            { $$ = make_node(NODE_SRL, 2, $1, $3) ; }
        | expr TOK_SRA expr
            { $$ = make_node(NODE_SRA, 2, $1, $3) ; }
        | expr TOK_SLL expr
            { $$ = make_node(NODE_SLL, 2, $1, $3) ; }
        | TOK_NOT expr
            { $$ = make_node(NODE_NOT, 1, $2) ; }
        | TOK_BNOT expr
            { $$ = make_node(NODE_BNOT, 1, $2) ; }
        | TOK_LPAR expr TOK_RPAR 
            {$$ = NULL ;}
        | ident TOK_AFFECT expr
            {$$ = make_node(NODE_AFFECT, 2, $1, $2);}
        | TOK_INTVAL
            {$$ = make_node(NODE_INTVAL, 1, $1);}
        | TOK_TRUE
            {$$ = make_node(NODE_TRUE, 1 $1);}
        | TOK_FALSE
            {$$ = make_node(NODE_FALSE, 1, $1);}
        | ident
            {$$ = make_node(NODE_IDENT, 1,, $1);}    
        ;

listparamprint: listparamprint TOK_COMMA paramprint
            { $$ = NULL; }
        | paramprint
            { $$ = NULL;}
        ;

paramprint: ident 
            { $$ = NULL;}
        | TOK_STRING 
            { $$ = NULL;}
        ;

ident:      TOK_IDENT
            { $$ = NULL;}
        ;

%%

/* A completer et/ou remplacer avec d'autres fonctions */
node_t make_node(node_nature nature, int nops, ...) {
    
    va_list ap;

    return NULL;
}


void analyse_tree(node_t root) {
    //dump_tree(root, "apres_syntaxe.dot");
    if (!stop_after_syntax) {
        analyse_passe_1(root);
        //dump_tree(root, "apres_passe_1.dot");
        if (!stop_after_verif) {
            create_program(); 
            gen_code_passe_2(root);
            dump_mips_program(outfile);
            free_program();
        }
        free_global_strings();
    }
    free_nodes(root);
}



/* Cette fonction est appelee automatiquement si une erreur de syntaxe est rencontree
 * N'appelez pas cette fonction vous-meme :    
 * la valeur donnee par yylineno ne sera plus correcte apres l'analyse syntaxique
 */
void yyerror(node_t * program_root, char * s) {
    fprintf(stderr, "Error line %d: %s\n", yylineno, s);
    exit(1);
}

