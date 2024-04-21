#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#include "defs.h"
#include "common.h"
#include "arch.h"


extern char * infile;
extern char * outfile;
int32_t trace_level = DEFAULT_TRACE_LEVEL;
extern bool stop_after_syntax;
extern bool stop_after_verif;

void parse_args(int argc, char ** argv) {
    //printf("Nombre d'arguments : %d", argc);
    //printf(argv[1]);
    FILE *fichier;
    infile = NULL;
    int fichierEntree = 0;
    int fichierSortie = 0;
    int v = 0;
    int s = 0; 
    int t = 0;
    int r = 0;
    
    //Sans option
    if(argc == 1){
        printf("Erreur : Aucun fichier et aucune option en argument.\n");
        printf("\n");
        printf("-b: Affiche une bannière indiquant le nom du compliateur et les membres du binome\n");
        printf("-o <nomFichier>: Definit le nom du fichier assembleur produit\n");
        printf("-t <int>: Definit le niveau de trace à utiliser entre 0 et 5 compris (0 par default)\n");
        printf("-r <int>: Definit le nombre maximum de registres à utiliser, entre 4 et 8 (8 par défaut)\n");
        printf("-s: Arreter la compilation apres l'analyse syntaxique (default = non)\n");
        printf("-v: Arreter la compilation apres la passe des verification (default = non)\n");
        printf("-h: Affiche la liste des options\n\n");
        printf("Les options '-s' et '-v' sont incompatibles\n");
        printf("-b doit être utilise seul\n");

        exit(1);
    }

    //Option -b
    if (strcmp(argv[1],"-b") == 0){
        if (argc >2){
            printf("L'option -b s'utilise seule !");
            printf("\n");
            exit(1);
        }
        for (int i = 0; i < 43; i++){
            printf("*");
        }
        printf("\n");
        for (int j = 0; j < 2; j++){
            printf("**");
            for (int i = 0; i < 39; i++){
                printf(" ");
            }
            printf("**");
            printf("\n");
        }
        printf("**   Youssef Chaoui Aziz, David Lamoot   **\n");
        printf("**            Juegos  Olympicos          **\n");
        for (int j = 0; j < 2; j++){
            printf("**");
            for (int i = 0; i < 39; i++){
                printf(" ");
            }
            printf("**");
            printf("\n");
        }
        for (int i = 0; i < 43; i++){
            printf("*");
        }
        printf("\n");
        exit(0);
    }

    for (int nb_arg = 1; nb_arg < argc; nb_arg++){
        //Option -h
        if (strcmp(argv[nb_arg],"-h") == 0){
            printf("-b: Affiche une bannière indiquant le nom du compliateur et les membres du binome\n");
            printf("-o: <nomFichier>: Definit le nom du fichier assembleur produit\n");
            printf("-t: <int>: Definit le niveau de trace à utiliser entre 0 et 5 compris (0 par default)\n");
            printf("-r: <int>: Definit le nombre maximum de registres à utiliser, entre 4 et 8 (8 par défaut)\n");
            printf("-s: Arreter la compilation apres l'analyse syntaxique (default = non)\n");
            printf("-v: Arreter la compilation apres la passe des verification (default = non)\n");
            printf("-h: Affiche la liste des options\n\n");
            printf("Les options '-s' et '-v' sont incompatibles\n");
            printf("-b doit être utilise seul\n");
            exit(0); //Arret du parsing des arguments
        }

        //Option -o <nomFichier>
        else if (strcmp(argv[nb_arg], "-o") == 0){
            /*if (fichierSortie >0){
                printf("Erreur : plus d'un fichier de sortie designe\n");
                exit(1);
            }*/
            if((nb_arg+1 == argc) || (argv[nb_arg+1][strlen(argv[nb_arg+1])-2] != '.') || (argv[nb_arg+1][strlen(argv[nb_arg+1])-1] != 's')){
                printf("Erreur : ligne de commande incorrecte\n");
                exit(1);
            }
            fichierSortie = 1;
            outfile = argv[nb_arg+1];
        }

        //Option -t <int>
        else if (strcmp(argv[nb_arg], "-t") == 0){
            char alpha = argv[nb_arg+1][strlen(argv[nb_arg+1])-1]; //test si chiffre ou lettre
            if (t > 0){
                printf("Erreur : impossible de definir plusieurs fois la trace \n");
            }
            else if ((nb_arg+1 == argc) || (atoi(argv[nb_arg+1]) < 0) || (atoi(argv[nb_arg+1]) > 5) || isalpha(alpha)){
                printf("Erreur : ligne de commande incorrecte\n");
                exit(1);
            }
            else{
                trace_level = atoi(argv[nb_arg+1]);
                t = 1;
            }
        }

        //Option -r <int>
        else if (strcmp(argv[nb_arg], "-r") == 0){
            if (r > 0){
                printf("Erreur : impossible de definir plusieurs fois le nombre maximum de registres \n");
                exit(1);
            }
            else if ((nb_arg+1 == argc) || (atoi(argv[nb_arg+1]) < 4) || (atoi(argv[nb_arg+1]) > 8)){
                printf("Erreur : ligne de commande incorrecte\n");
                exit(1);
            }
            else{
                set_max_registers(atoi(argv[nb_arg+1]));
                r = 1;
            }
        }

        //Option -s
        else if (strcmp(argv[nb_arg], "-s") == 0){
            if ((v > 0) || (s>0)){
                printf("Erreur : ligne de commande incorrecte\n");
                exit(1);
            }
            stop_after_syntax = 1;
            s = 1;
        }

        //Option -v
        else if (strcmp(argv[nb_arg], "-v") == 0){
            if ((v > 0) || (s>0)){
                printf("Erreur : ligne de commande incorrecte\n");
                exit(1);
            }
            stop_after_verif = 1;
            v = 1;
        }

        //Infile
        else if ((argv[nb_arg][strlen(argv[nb_arg])-2] == '.')){
            if ((argv[nb_arg][strlen(argv[nb_arg])-1] == 'c')){
                if(fichierEntree > 0){
                    fichierEntree++;
                }
                else{
                infile = argv[nb_arg];
                fichierEntree = 1;
                fichier = fopen(infile, "r");
                if(fichier == NULL){
                    printf("Le fichier n'existe pas\n");
                    exit(1);
                }
                fclose(fichier);
                }
            }
        } 


    }
    if (infile == NULL){
        printf("Erreur : aucun fichier en entree\n");
        exit(1);
    }

    if (fichierEntree > 1){
        printf("Erreur : %d fichiers en entrée\n", fichierEntree);
        exit(1);
    }
}



void free_nodes(node_t n) {

    int i = 0;
    if(n == NULL){
        return;
    }
   while(i<n->nops){
   if(n->opr[i]!=NULL){
        free_nodes(n->opr[i]);
        n->opr[i]=NULL;
        
    }
   i++;
   }

    free(n->str);
    n->str=NULL;
    free(n->ident);
    n->ident=NULL;
   free(n);
}


char * strdupl(char * s) {
    char * r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}


static int32_t dump_tree2dot_rec(FILE * f, node_t n, int32_t node_num) {

    if (n == NULL) {
        fprintf(f, "    N%d [shape=record, label=\"{{NULL}}\"];\n", node_num);
        return node_num;
    }

    switch (n->nature) {
        case NODE_IDENT:
            {
                node_t decl_node = n->decl_node;
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{<decl>Decl      |Ident: %s|Offset: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->ident, n->offset);
                if (decl_node != NULL && decl_node != n) {
                    fprintf(f, "    edge[tailclip=false];\n");
                    fprintf(f, "    \"N%d\":decl:c -> \"N%d\" [style=dashed]\n", node_num, decl_node->node_num);
                }
                break;
            }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{Value: %" PRId64 "}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->value);
            break;
        case NODE_STRINGVAL:
            {
                char str[32];
                int32_t i = 1;
                while (true) {
                    str[i - 1] = n->str[i];
                    i += 1;
                    if (n->str[i] == '"') {
                        str[i - 1] = '\0';
                        break;
                    }
                }
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{val: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), str);
            }
            break;
        case NODE_TYPE:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type));
            break;
        case NODE_LIST:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE LIST}}\"];\n", node_num);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PRINT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops);
            break;
        case NODE_FUNC:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}|{offset: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops, n->offset);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRA:
        case NODE_SRL:
        case NODE_SLL:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_UMINUS:
        case NODE_AFFECT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->nops);
            break;
        default:
            printf("*** Error in %s: unknow nature : %s\n", __func__, node_nature2string(n->nature));
            assert(false);
    }

    n->node_num = node_num;

    int32_t curr_node_num = node_num + 1;
    for (int32_t i = 0; i < n->nops; i += 1) {
        int32_t new_node_num = dump_tree2dot_rec(f, n->opr[i], curr_node_num);

        fprintf(f, "    edge[tailclip=true];\n");
        fprintf(f, "    N%d -> N%d\n", node_num, curr_node_num);
        curr_node_num = new_node_num + 1;
    }

    return curr_node_num - 1;
}



static void dump_tree2dot(FILE * f, node_t root) {
    assert(root->nature == NODE_PROGRAM);

    int32_t curr_node_num = 1;
    dump_tree2dot_rec(f, root, curr_node_num);
}


void dump_tree(node_t prog_root, const char * dotname) {

    FILE * f;

    f = fopen(dotname, "w");
    fprintf(f, "digraph global_vars {\n");
    dump_tree2dot(f, prog_root);
    fprintf(f, "}");    
    fclose(f);
}


const char * node_type2string(node_type t) {
    switch (t) {
        case TYPE_NONE:
            return "TYPE NONE";
        case TYPE_INT:
            return "TYPE INT";
        case TYPE_BOOL:
            return "TYPE BOOL";
        case TYPE_VOID:
            return "TYPE VOID";
        default:
            assert(false);
    }
}


const char * node_nature2string(node_nature t) {
    switch (t) {
        case NONE:
            return "NONE";
        case NODE_PROGRAM:
            return "PROGRAM";
        case NODE_BLOCK:
            return "BLOCK";
        case NODE_LIST:
            return "LIST";
        case NODE_DECLS:
            return "DECLS";
        case NODE_DECL:
            return "DECL";
        case NODE_IDENT:
            return "IDENT";
        case NODE_TYPE:
            return "TYPE";
        case NODE_INTVAL:
            return "INTVAL";
        case NODE_BOOLVAL:
            return "BOOLVAL";
        case NODE_STRINGVAL:
            return "STRINGVAL";
        case NODE_FUNC:
            return "FUNC";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_DOWHILE:
            return "DOWHILE";
        case NODE_PLUS:
            return "PLUS";
        case NODE_MINUS:
            return "MINUS";
        case NODE_MUL:
            return "MUL";
        case NODE_DIV:
            return "DIV";
        case NODE_MOD:
            return "MOD";
        case NODE_LT:
            return "LT";
        case NODE_GT:
            return "GT";
        case NODE_LE:
            return "LE";
        case NODE_GE:
            return "GE";
        case NODE_EQ:
            return "EQ";
        case NODE_NE:
            return "NE";
        case NODE_AND:
            return "AND";
        case NODE_OR:
            return "OR";
        case NODE_BAND:
            return "BAND";
        case NODE_BOR:
            return "BOR";
        case NODE_BXOR:
            return "BXOR";
        case NODE_SLL:
            return "SLL";
        case NODE_SRA:
            return "SRA";
        case NODE_SRL:
            return "SRL";
        case NODE_NOT:
            return "NOT";
        case NODE_BNOT:
            return "BNOT";
        case NODE_UMINUS:
            return "UMINUS";
        case NODE_AFFECT:
            return "AFFECT";
        case NODE_PRINT:
            return "PRINT";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




const char * node_nature2symb(node_nature t) {
    switch (t) {
        case NONE:
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_MUL:
            return "*";
        case NODE_DIV:
            return "/";
        case NODE_MOD:
            return "%";
        case NODE_LT:
            return "<";
        case NODE_GT:
            return ">";
        case NODE_LE:
            return "<=";
        case NODE_GE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NE:
            return "!=";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_BAND:
            return "&";
        case NODE_BOR:
            return "|";
        case NODE_BXOR:
            return "^";
        case NODE_SRA:
            return ">>";
        case NODE_SRL:
            return ">>>";
        case NODE_SLL:
            return "<<";
        case NODE_NOT:
            return "!";
        case NODE_BNOT:
            return "~";
        case NODE_UMINUS:
            return "-";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




