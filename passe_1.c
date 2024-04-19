
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "passe_1.h"
#include "utils/miniccutils.h"
#include "common.h"
#include "string.h"

static bool global =true; //semaphore pour passer ou non le global_decl du node a true ou false et changé lors de l'entrée dans maindelc


int analyse_decl(bool global,node_t node,node_type type ){
    if(global){
    //vérifier que la variable est initialisée par une constante et pas une autre variable int a = b;
    if(node->opr[1]->nature==NODE_BOOLVAL || node->opr[1]->nature==NODE_INTVAL ||node->opr[1]==NULL ){

    
    node->type = type; // faire passer le type à travers l'arbre sur les noeux ident
    env_add_element(node->ident,node); 
    
    }
    else{

        printf("erreur line %d: syntax error", node->lineno);
    }
    


    }
    else{
        //dans ce cas verif que var fait verif type et tout jsp si c fait dans get_decl_node

        //if(get_decl_node()) //verifier si la var est init avec une autre var que cette var utilisée esxiste
        node->type = node->decl_node->type;

        } //dans le cas ou on à int c =d ; avec d deja défini dans le contexte il faut remonter à la valeur de c connue dans le contexte

            

    }

    
   
    

    
    
//propager le type oui mais pas partout que ident intval littéraux.

//parcours global parcours local et parcours expr juse global en vrai


void parcours_global_arbre(node_t root){


    //printf("%s\n",root->nature);
    if(root==NULL){
        return;
    }
    printf("%s\t",node_nature2string(root->nature));
    //printf(" %s \t",node_nature2string(root->nature));
    
    switch(root->nature){
        case(NODE_LIST):
        
        //jsp quoi faire considérer comme un sous arbre et traiter list comme le sommet et répéter l'opération
       
        if(root->opr[0]!=NULL){
            root->opr[0]->type=root->type;
            parcours_global_arbre(root->opr[0]);
        }
        if(root->opr[1]!=NULL){
            root->opr[1]->type=root->type;
            parcours_global_arbre(root->opr[1]);
        
        } //penser à transmettre le type ici aussi

            break;
        case(NODE_PROGRAM):
        push_global_context();
        if(root->opr[0]!=NULL){
            parcours_global_arbre(root->opr[0]);


        }
        if(root->opr[1]!=NULL){
        parcours_global_arbre(root->opr[1]);
        }
        pop_context();
        break;

        case(NODE_DECLS):
       //peut causer
       if(root->opr[0]->type==TYPE_VOID){
        printf("Error line %d: void is not a valid variable type",root->lineno);
        exit(-1);
       }
        root->opr[1]->type = root->opr[0]->type ;
        //printf("%d\t",root->opr[1]->type);
        parcours_global_arbre(root->opr[1]);
        
        //récupérer le type pour le transmettre
            //int type = root->opr[0]->type;
            break;
        case(NODE_FUNC):
            global = false;
            reset_env_current_offset(); 
            if(root->opr[0]->type!=TYPE_VOID){
                printf("Error line %d: wrong return type", root->opr[0]->lineno);
                exit(-1);
                
            }
            if(strcmp(root->opr[1]->ident,"main")!=0){
                printf("Error line %d: wrong function name", root->opr[1]->lineno);
                exit(-1);

            }
            parcours_global_arbre(root->opr[1]);   
            parcours_global_arbre(root->opr[2]);
            root->offset=get_env_current_offset();

            break;                
        
        case(NODE_DECL):
                //pas forcément nécessaire on peut le traiter d'au dessus avec root->opr[i]
            
                
             root->opr[0]->type=root->type;
             root->opr[0]->offset = env_add_element(root->opr[0]->ident,root->opr[0]);
             if(root->opr[0]->offset)
            //if(root->opr[1]->nature==NODE_IDENT){
                //root->opr[1]->type=root->type;
            //}        
            
            //printf("%d\t",root->type);
            parcours_global_arbre(root->opr[0]);
            if(root->opr[1]!=NULL){

            root->opr[1]->type=root->type;
            parcours_global_arbre(root->opr[1]); //rajouter intval
            
            }
               


        break;

        case(NODE_IDENT): //appeler analyse_decl ici    
        //analyse_decl(global,root,type);
        root->global_decl = global;
        root->decl_node=root;
        //printf("%d   %s",global,root->ident);
        //root->offset = env_add_element(root->ident,root);
        if(root->global_decl==0){
            root->decl_node = get_decl_node(root->ident);
            if(root->decl_node!=NULL){
                root->type=root->decl_node->type;
            }
            

            //root->type = root->decl_node->type;
       // printf(" %s  ",root->decl_node->ident);
        }
        //printf("%d\t",root->type);

        //get_env_current_offset();    
        //push_context(); 
        //printf(" %s \n",root->ident);
            break;

        case(NODE_BLOCK):
        push_context();
        if(root->opr[0]!=NULL){
        
        parcours_global_arbre(root->opr[0]);
        }
        if(root->opr[1]!=NULL){
        parcours_global_arbre(root->opr[1]);
        
        }
        pop_context();
            break;

        case(NODE_FOR):
        
        parcours_global_arbre(root->opr[0]);
        parcours_global_arbre(root->opr[1]);
        parcours_global_arbre(root->opr[2]);
        if(root->opr[3]!=NULL){
        parcours_global_arbre(root->opr[3]);
        }
            break;

        case(NODE_IF):
        parcours_global_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
        parcours_global_arbre(root->opr[1]);
        }

        if(root->opr[2]!=NULL){
        parcours_global_arbre(root->opr[2]);
        }

        break;

        case(NODE_WHILE):
        parcours_global_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
                parcours_global_arbre(root->opr[1]);
        }

        break;
        case(NODE_DOWHILE):
        if(root->opr[0]!=NULL){
        parcours_global_arbre(root->opr[0]);
        }
        parcours_global_arbre(root->opr[1]);
        break;

        case(NODE_AFFECT):

        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        /*if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }*/

        break;

        case(NODE_LT):
        
        parcours_global_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

        

        break;

        case(NODE_GT):

        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;


        case(NODE_LE):
        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;
     
        case(NODE_GE):
        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;
 
        case(NODE_BOR):
        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;

        case(NODE_PLUS):
        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

        if(root->opr[0]->type!=TYPE_INT||root->opr[1]->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }


        break;

        case(NODE_MINUS):
        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
        
        }

         if(root->opr[0]->type!=TYPE_INT||root->opr[1]->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }
        



        break;

        case(NODE_MUL):
        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

         if(root->opr[0]->type!=TYPE_INT||root->opr[1]->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }


        break;

        case(NODE_DIV):
        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

         if(root->opr[0]->type!=TYPE_INT||root->opr[1]->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }


        break;

        case(NODE_MOD):
        
        parcours_global_arbre(root->opr[0]);
        root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type ){
        printf("Error line %d: les variables doivent être de même type\n",root->lineno);
        exit(-1);
            
        }
         if(root->opr[0]->type!=TYPE_INT || root->opr[1]->type!=TYPE_INT ){
        printf("Error line %d: modulo sur les entiers seulement\n",root->lineno);
        exit(-1);
            
        }

        break;


        case(NODE_EQ):
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
         if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

        break;

        case(NODE_NE):
        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

        break;

        case(NODE_AND):
        
        parcours_global_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;

        case(NODE_BAND):
        
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;

        case(NODE_BNOT):
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;

          if(root->opr[0]->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }
        


        break;
        
        case(NODE_BXOR):
        parcours_global_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_global_arbre(root->opr[1]);
        
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;

        case(NODE_NOT):
        parcours_global_arbre(root->opr[0]);
          root->type=TYPE_BOOL;
        
            
    

            
        


        break;

        case(NODE_SLL):
          parcours_global_arbre(root->opr[0]);
           
        parcours_global_arbre(root->opr[1]);


        break;

        case(NODE_SRL):
          parcours_global_arbre(root->opr[0]);
          root->type=root->opr[0]->type;
        parcours_global_arbre(root->opr[1]);
        if(root->opr[0]->type!=root->opr[1]->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }


        break;

        case(NODE_UMINUS):
          parcours_global_arbre(root->opr[0]);
          root->type=root->opr[0]->type;
          if(root->opr[0]->type!=TYPE_INT){
            printf("Error line %d: moins unaire sur entier uniquement ",root->lineno);
          }
        break;

        case(NODE_PRINT):
          parcours_global_arbre(root->opr[0]);

        break;

        case(NODE_INTVAL):
            break;

        case(NODE_STRINGVAL):
        root->offset = add_string(root->str);
            break;

        case(NODE_BOOLVAL):
            break;

        case(NODE_TYPE):
            break;
        
         


        



        









        /*case(NODE_INTVAL): //pas important imo quoique
            break;
        
        case(NODE_BOOLVAL): //pareil
            break;
        
        case(NODE_STRINGVAL): //important appeler la fonction add_string et prévoir un traitement mémoire spécial
            break;              // pour les strings sinon problèmes
        
*/

    
        default:
        break;
    }
        


          
         
    

    }






    
int analyse_func(node_t node){ //vérifier que le node main est bien de type void
if(node->opr[0]->type == TYPE_VOID){
    printf("line error %d", node->lineno);

}
//reset_env_current_offset();
//push_context();
node->offset =get_env_current_offset(); //mettre l'offset total dans le champ offset du noeud node_fonc 
//ATTENTION C'est L'OFFSET A LA FIN DU PARCOURS DE L'ARBRE voir comment le maj a  la fin

 //a faire en entrant dans le main
//rappeler les decls


    

}

//entrée dans un bloc d'instruction empiler a nouveau le contexte

//vérifier que le type dans un while if for etc est bien un bool



int gerer_instructions(){ //suivant le retour on sait si erreur ou pas
//push_context(); //à faire au début du bloc
//dans un égal chercher avec get_decl_node pour ajouter la valeur

pop_context(); //à faire en fin de bloc dépiler le contexte
//peut être pas dans cette fonction en vrai
    




}

int gerer_var(){ //SI BOOL A VAL AUTRE QUE 0 OU 1 ERREUR vérifications
//appeler getdecl_node et 

}

    


extern int trace_level;
node_type type;
void analyse_passe_1(node_t root) {
     //à faire au début pour avoir le contexte global
    parcours_global_arbre(root);    
    printf("\n");
    }
    
