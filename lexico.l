%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "defs.h"
#include "common.h"

#include "y.tab.h"

int yyparse(node_t * program_root);
void analyse_tree(node_t root);


char * infile = NULL;
char * outfile = DEFAULT_OUTFILE;
bool stop_after_syntax = false;
bool stop_after_verif = false;

#if YYDEBUG
extern int yydebug;
#endif

#if LEX_DEBUG
#define RETURN(token) ({ printf("%s \t\"%s\"\n", #token, yytext); return token; })
#else
#define RETURN(token) ({ return token; })
#endif


%}

%option yylineno


LETTRE          [a-zA-Z]

ENTIER        [0-9]
IDF             {LETTRE}({LETTRE}|{ENTIER}|_)*
CHAINE          \"[^"]*\"
COMMENTAIRE     "//"({LETTRE}|{ENTIER})*

%%
      
"void"          RETURN(TOK_VOID);
"int"           RETURN(TOK_INT);
"true"          RETURN(TOK_TRUE);
"false"         RETURN(TOK_FALSE);
"bool"          RETURN(TOK_BOOL);
"if"            RETURN(TOK_IF);
"else"          RETURN(TOK_ELSE);
"while"         RETURN(TOK_WHILE);
"for"           RETURN(TOK_FOR);
"print"         RETURN(TOK_PRINT);
"do"            RETURN(TOK_DO);




{IDF} {
                #if !LEX_DEBUG
                yylval.strval = strdupl(yytext);

                #endif
                RETURN(TOK_IDENT);
}

{ENTIER} {
                #if !LEX_DEBUG
                /* A completer : affecter yylval.intval et faire des verifications */
                yylval.intval = atoi(yytext);
                #endif
                RETURN(TOK_INTVAL);
}

{CHAINE} {
                #if !LEX_DEBUG
                /* A completer : affecter yylval.strval */
                yylval.strval = strdupl(yytext);

                #endif
                RETURN(TOK_STRING);
}

{COMMENTAIRE} {
    
}


[ \t\n]+

. {
                fprintf(stderr, "Error line %d: Lexical error\n", yylineno);
                exit(1);
}


%%

int yywrap(void) {
    return 1;
}


int main(int argc, char ** argv) {
    node_t program_root;
    parse_args(argc, argv);
    yyin = fopen(infile, "r");
    #if LEX_DEBUG
        while(yylex());
        fclose(yyin);
    #else
        #if YYDEBUG
        yydebug = 1;
        #endif
        yyparse(&program_root);
        fclose(yyin);
        analyse_tree(program_root);
    #endif
    yylex_destroy();
    return 0;
}





