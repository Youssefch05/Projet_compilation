
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "passe_1.h"
#include "utils/miniccutils.h"
#include "common.h"
#include "string.h"

void verif_declaration_unaire(node_t root){
    if(root->opr[0]->decl_node==NULL && root->opr[0]->nature==NODE_IDENT){
            printf("Error line %d: undeclared variable\n",root->lineno);
                exit(1); 
                }
}
void verif_decla_binaire(node_t root){
    if((root->opr[0]->decl_node==NULL && root->opr[0]->nature==NODE_IDENT) || (root->opr[1]->decl_node==NULL&&root->opr[1]->nature==NODE_IDENT)){
            printf("Error line %d: variable non déclarée\n",root->lineno);
            exit(1);

       }   
}


void verif_type_unaire(node_t root){
    if(root->opr[0]->decl_node!=NULL){
          if(root->opr[0]->decl_node->type!=TYPE_INT){
            printf("Error line %d: opération unaire sur entier uniquement\n ",root->lineno);
            exit(-1);
          }
          } 
          else{
            if(root->opr[0]->type!=TYPE_INT){
            printf("Error line %d: moins unaire sur entier uniquement\n ",root->lineno);
            exit(-1);

          }
          }
        
}

void verif_type_unaire_bool(node_t root){
    if(root->opr[0]->decl_node!=NULL){
          if(root->opr[0]->decl_node->type!=TYPE_BOOL){
            printf("Error line %d: opération unaire sur entier uniquement\n ",root->lineno);
            exit(-1);
          }
          } 
          else{
            if(root->opr[0]->type!=TYPE_BOOL){
            printf("Error line %d: moins unaire sur entier uniquement\n ",root->lineno);
            exit(-1);

          }
          }
        
}

void verif_condition_bool_opr1(node_t root){
    if(root->opr[1]->type!=TYPE_BOOL){
            printf("Error line %d: condition booleene nécessaire\n",root->lineno);
            exit(-1);
        }

}

void verif_condition_bool_opr0(node_t root){
    if(root->opr[0]->type!=TYPE_BOOL){
            printf("Error line %d: condition booleene nécessaire\n",root->lineno);
            exit(-1);
        }

}



void verif_type_binaire_int(node_t root){
    if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            
        }
        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            }
            }   
        }

}

void verif_type_binaire_bool(node_t root){
    if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node\n",root->lineno);
        exit(-1);
            
        }
        
        }
        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_BOOL){

                     printf("Error line %d: types should be identical in node\n",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_BOOL){
                   printf("Error line %d: types should be identical in node\n",root->lineno);
        exit(-1);
            }

        }
        }

}

static bool global =true; //semaphore pour passer ou non le global_decl du node a true ou false et changé lors de l'entrée dans maindelc

void parcours_arbre(node_t root){


    if(root==NULL){
        return;
    }
 
    
    switch(root->nature){
        case(NODE_LIST):
        
       
        if(root->opr[0]!=NULL){
            root->opr[0]->type=root->type;
            parcours_arbre(root->opr[0]);
        }
        if(root->opr[1]!=NULL){
            root->opr[1]->type=root->type;
            parcours_arbre(root->opr[1]);
        
        } 

            break;
        case(NODE_PROGRAM):
        push_global_context();
        if(root->opr[0]!=NULL){
            parcours_arbre(root->opr[0]);


        }
        if(root->opr[1]!=NULL){
        parcours_arbre(root->opr[1]);
        }
        pop_context();
        break;

        case(NODE_DECLS):
      
       if(root->opr[0]->type==TYPE_VOID){
        printf("Error line %d: void is not a valid variable type\n",root->lineno);
        exit(-1);
       }
        root->opr[1]->type = root->opr[0]->type ;
        //printf("%d\t",root->opr[1]->type);
        parcours_arbre(root->opr[1]);
        
      
            break;
        case(NODE_FUNC):
            global = false;
            reset_env_current_offset(); 
            if(root->opr[0]->type!=TYPE_VOID){
                printf("Error line %d: wrong return type\n", root->opr[0]->lineno);
                exit(-1);
                
            }
            if(strcmp(root->opr[1]->ident,"main")!=0){
                printf("Error line %d: wrong function name\n", root->opr[1]->lineno);
                exit(-1);

            }
            parcours_arbre(root->opr[1]);   
            parcours_arbre(root->opr[2]);
            root->offset=get_env_current_offset();

            break;                
        
        case(NODE_DECL):
               
            
                
             root->opr[0]->type=root->type;
             root->opr[0]->offset = env_add_element(root->opr[0]->ident,root->opr[0]);
             if(root->opr[0]->offset)
          
            parcours_arbre(root->opr[0]);
            if(root->opr[1]!=NULL){

            root->opr[1]->type=root->type;
            parcours_arbre(root->opr[1]); 
            
            }
            if(root->opr[1]!=NULL){
                
             if((root->opr[1]->nature==NODE_IDENT)&&!(strcmp(root->opr[1]->ident,root->opr[0]->ident))){
                printf("Error line %d: une variable ne se déclare pas elle même\n",root->lineno);
                exit(1);
             }

              if(root->opr[1]->decl_node!=NULL){      
              if(root->opr[1]->decl_node->type!=root->opr[0]->type){
                printf("Error line %d: type de variable incorrect\n",root->lineno);
              }  
             }
             
            }
        
            if(root->opr[0]!=NULL && root->opr[1]!=NULL){
            if(root->opr[0]->type!=root->opr[1]->type && global==1){
                printf("Error line %d: global var only with literal\n",root->lineno);
                exit(-1);
            
            
            }

            if(root->opr[1]->nature==NODE_IDENT && global==1){
                printf("Error line %d: global var only with literral\n",root->lineno);
                exit(-1);
            
            
            }

            if(root->opr[1]->nature==NODE_IDENT&&root->opr[1]->decl_node==NULL&&global==0){

                printf("Error line %d: variable non déclarée\n",root->lineno);
                exit(1);

            }

            if((root->opr[0]->type==TYPE_BOOL && ((root->opr[1]->value!= true) && (root->opr[1]->value!=false)))){
                
                printf("Error line %d: unvalid value for bool\n", root->lineno);
                exit(1);
            }

            
        
            }
            

        break;

        case(NODE_IDENT):  
        
        root->global_decl = global;
        root->decl_node=root;
        if(root->global_decl==0){
            root->decl_node = get_decl_node(root->ident);
           
                
        }
        
        
            break;

        case(NODE_BLOCK):
        push_context();
        if(root->opr[0]!=NULL){
        
        parcours_arbre(root->opr[0]);
        }
        if(root->opr[1]!=NULL){
        parcours_arbre(root->opr[1]);
        
        }
        pop_context();
            break;

        case(NODE_FOR):
        
        parcours_arbre(root->opr[0]);
        parcours_arbre(root->opr[1]);
        parcours_arbre(root->opr[2]);
        if(root->opr[3]!=NULL){
        parcours_arbre(root->opr[3]);
        }
        verif_condition_bool_opr1(root);
            break;

    
        case(NODE_IF):
        parcours_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
        parcours_arbre(root->opr[1]);
        }

        if(root->opr[2]!=NULL){
        parcours_arbre(root->opr[2]);
        }
        verif_condition_bool_opr0(root);

        break;

        case(NODE_WHILE):
        parcours_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
                parcours_arbre(root->opr[1]);
        }
        verif_condition_bool_opr0(root);

        break;
        case(NODE_DOWHILE):
        if(root->opr[0]!=NULL){
        parcours_arbre(root->opr[0]);
        }
        parcours_arbre(root->opr[1]);
         verif_condition_bool_opr1(root);
        break;

        case(NODE_AFFECT):

        
        parcours_arbre(root->opr[0]);
        
        if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
        }
        
        parcours_arbre(root->opr[1]);

        if(root->opr[0]->nature==NODE_IDENT&&root->opr[0]->decl_node==NULL){

                printf("Error line %d: variable non déclarée\n",root->lineno);
                exit(-1);

            }
        
        
        if(root->opr[1]->nature==NODE_IDENT&&root->opr[1]->decl_node==NULL){

                printf("Error line %d: variable non déclarée\n",root->lineno);
                exit(-1);

            }
        
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        
        printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            
        }

        }
        else{
            if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=root->opr[1]->type){
                if(root->opr[0]->offset==-1){

                }
                printf("Error line %d: types should be identical\n",root->lineno);
                exit(-1);
            
        }

            }
            }
        
    
        break;

        case(NODE_LT):
        parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        
        verif_decla_binaire(root);
        verif_type_binaire_int(root);
        

        break;

        case(NODE_GT):

        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
         
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);


        break;


        case(NODE_LE):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
         
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root); 


        break;
     
        case(NODE_GE):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
        parcours_arbre(root->opr[1]);
          
        verif_decla_binaire(root);  
        verif_type_binaire_int(root);
        break;
 
        case(NODE_BOR):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
         verif_type_binaire_int(root);

        break;

        case(NODE_PLUS):
        
        parcours_arbre(root->opr[0]);
        if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
        }
        
        
        parcours_arbre(root->opr[1]);
         if(root->opr[1]->decl_node!=NULL){
            root->type = root->opr[1]->decl_node->type;
    }
        verif_decla_binaire(root);
       verif_type_binaire_int(root);
       


        break;

        case(NODE_MINUS):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);


        break;

        case(NODE_MUL):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);

        break;

        case(NODE_DIV):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);        
        verif_type_binaire_int(root);

    
        

        break;

        case(NODE_MOD):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
       verif_type_binaire_int(root);
        break;


        case(NODE_EQ):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
         if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
         if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            
        }
         }

        break;

        case(NODE_NE):
        
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
         if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        printf("Error line %d: types should be identical\n",root->lineno);
        exit(-1);
            
        }
        }

        break;

        case(NODE_AND):
        
        parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        
        verif_type_binaire_bool(root);

        break;

        case(NODE_OR):
             parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
       verif_type_binaire_bool(root);

        break;

        case(NODE_BAND):
        
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);


        break;

        case(NODE_BNOT):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
         verif_declaration_unaire(root);
            verif_type_unaire(root);
    


        break;
        
        case(NODE_BXOR):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);
        


        break;

        case(NODE_NOT):
        parcours_arbre(root->opr[0]);
          root->type=TYPE_BOOL;
          verif_declaration_unaire(root);
        verif_type_unaire_bool(root);
        

        
          verif_declaration_unaire(root);
          if(root->opr[0]->nature==NODE_INTVAL){
            printf("Error line %d: negation of intval not possible \n",root->lineno);
            exit(-1);
          }
          
             
        break;

        case(NODE_SLL):
          parcours_arbre(root->opr[0]);
           if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
           
        parcours_arbre(root->opr[1]);

        verif_decla_binaire(root);
        verif_type_binaire_int(root);
            
        
        break;

        case(NODE_SRL):
          parcours_arbre(root->opr[0]);
           if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);

        case(NODE_SRA):
         parcours_arbre(root->opr[0]);
          if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        verif_decla_binaire(root);
        verif_type_binaire_int(root);

        

        break;

        case(NODE_UMINUS):
          parcours_arbre(root->opr[0]);
          root->type=root->opr[0]->type;
          verif_declaration_unaire(root);
          verif_type_unaire(root);
           
        break;

        case(NODE_PRINT):
          parcours_arbre(root->opr[0]);

        break;

        case(NODE_INTVAL):
        root->type = TYPE_INT;              
            break;

        case(NODE_STRINGVAL):
        root->offset = add_string(root->str);
            break;

        case(NODE_BOOLVAL):
            break;

        case(NODE_TYPE):
            break;
 
        default:
        break;
    
    }      
    }

extern int trace_level;
node_type type;
void analyse_passe_1(node_t root) {
     //à faire au début pour avoir le contexte global
    parcours_arbre(root);    
    }
    
