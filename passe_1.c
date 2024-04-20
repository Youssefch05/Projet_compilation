
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "passe_1.h"
#include "utils/miniccutils.h"
#include "common.h"
#include "string.h"

static bool global =true; //semaphore pour passer ou non le global_decl du node a true ou false et changé lors de l'entrée dans maindelc

//propager le type oui mais pas partout que ident intval littéraux.

//parcours global parcours local et parcours expr juse global en vrai


void parcours_arbre(node_t root){


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
            parcours_arbre(root->opr[0]);
        }
        if(root->opr[1]!=NULL){
            root->opr[1]->type=root->type;
            parcours_arbre(root->opr[1]);
        
        } //penser à transmettre le type ici aussi

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
       //peut causer
       if(root->opr[0]->type==TYPE_VOID){
        printf("Error line %d: void is not a valid variable type",root->lineno);
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
                printf("Error line %d: wrong return type", root->opr[0]->lineno);
                exit(-1);
                
            }
            if(strcmp(root->opr[1]->ident,"main")!=0){
                printf("Error line %d: wrong function name", root->opr[1]->lineno);
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
            parcours_arbre(root->opr[1]); //rajouter intval
            
            }
            if(root->opr[1]!=NULL){
                
             if((root->opr[1]->nature==NODE_IDENT)&&!(strcmp(root->opr[1]->ident,root->opr[0]->ident))){
                printf("Error line %d: une variable ne se déclare pas elle même",root->lineno);
                exit(1);
             }

              if(root->opr[1]->decl_node!=NULL){      
              if(root->opr[1]->decl_node->type!=root->opr[0]->type){
                printf("Error line %d: type de variable incorrect",root->lineno);
              }  
             }
             
            }
        
            if(root->opr[0]!=NULL && root->opr[1]!=NULL){
            if(root->opr[0]->type!=root->opr[1]->type && global==1){
                printf("Error line %d: global var only with literal",root->lineno);
                exit(-1);
            
            
            }

            if(root->opr[1]->nature==NODE_IDENT && global==1){
                printf("Error line %d: global var only with literral",root->lineno);
                exit(-1);
            
            
            }

            if(root->opr[1]->nature==NODE_IDENT&&root->opr[1]->decl_node==NULL&&global==0){

                printf("Error line %d: variable non déclarée",root->lineno);
                exit(1);

            }

            if((root->opr[0]->type==TYPE_BOOL && ((root->opr[1]->value!= true) && (root->opr[1]->value!=false)))){
                
                printf("Error line %d: unvalid value for bool", root->lineno);
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
        if(root->opr[1]->type!=TYPE_BOOL){
            printf("Error line %d: condition booleene nécessaire",root->lineno);
            exit(-1);
        }
            break;

    
        case(NODE_IF):
        parcours_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
        parcours_arbre(root->opr[1]);
        }

        if(root->opr[2]!=NULL){
        parcours_arbre(root->opr[2]);
        }
        if(root->opr[0]->type!=TYPE_BOOL){
            printf("Error line %d: condition de type booleene ", root->lineno);
            exit(-1);
        }

        break;

        case(NODE_WHILE):
        parcours_arbre(root->opr[0]);
        if(root->opr[1]!=NULL){
                parcours_arbre(root->opr[1]);
        }
        if(root->opr[0]->type!=TYPE_BOOL){
            printf("Error line %d: condition de type booleene ", root->lineno);
            exit(-1);
        }

        break;
        case(NODE_DOWHILE):
        if(root->opr[0]!=NULL){
        parcours_arbre(root->opr[0]);
        }
        parcours_arbre(root->opr[1]);
         if(root->opr[1]->type!=TYPE_BOOL){
            printf("Error line %d: condition de type booleene ", root->lineno);
            exit(-1);
        }
        break;

        case(NODE_AFFECT):

        
        parcours_arbre(root->opr[0]);
        
        if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
        }
        
        parcours_arbre(root->opr[1]);

        if(root->opr[0]->nature==NODE_IDENT&&root->opr[0]->decl_node==NULL){

                printf("Error line %d: variable non déclarée",root->lineno);
                exit(-1);

            }
        


        
            
        

        
        if(root->opr[1]->nature==NODE_IDENT&&root->opr[1]->decl_node==NULL){

                printf("Error line %d: variable non déclarée",root->lineno);
                exit(-1);

            }
        
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }

        }
        else{
            if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=root->opr[1]->type){
                if(root->opr[0]->offset==-1){

                }
                printf("Error line %d: types should be identical",root->lineno);
                exit(-1);
            
        
        }

            }
            }
        
        



        break;

        case(NODE_LT):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
            }   
        }


        break;

        case(NODE_GT):

        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
         
        parcours_arbre(root->opr[1]);
         if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }


        break;


        case(NODE_LE):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
         
        parcours_arbre(root->opr[1]);
          if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }


        break;
     
        case(NODE_GE):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
        parcours_arbre(root->opr[1]);
          if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
        


        break;
 
        case(NODE_BOR):
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[0]);
        parcours_arbre(root->opr[1]);
         if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }

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
        
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
            }

        }


        break;

        case(NODE_MINUS):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }


        break;

        case(NODE_MUL):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
        break;

        case(NODE_DIV):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
                
       if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);

        
            
        }
       

        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }

    
        

        break;

        case(NODE_MOD):
        
        parcours_arbre(root->opr[0]);
         if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
        break;


        case(NODE_EQ):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
         if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
         if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }
         }
        break;

        case(NODE_NE):
        
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
         if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){
        printf("Error line %d: types should be identical",root->lineno);
        exit(-1);
            
        }
        }

        break;

        case(NODE_AND):
        
        parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        


        }
        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_BOOL){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_BOOL){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }

        
        

        break;

        case(NODE_OR):
             parcours_arbre(root->opr[0]);
        root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        


        }
        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_BOOL){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_BOOL){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }

        break;

        case(NODE_BAND):
        
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
        


        break;

        case(NODE_BNOT):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        if(root->opr[0]->decl_node!=NULL){
          if(root->opr[0]->decl_node->type!=TYPE_INT){
            printf("Error line %d: opération uniquement sur les int",root->lineno);
            exit(-1);
            
        }
        }


        break;
        
        case(NODE_BXOR):
        parcours_arbre(root->opr[0]);
         root->type=TYPE_BOOL;
        parcours_arbre(root->opr[1]);
        
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
            
        


        break;

        case(NODE_NOT):
        parcours_arbre(root->opr[0]);
          root->type=TYPE_BOOL;
        if(root->opr[0]->decl_node!=NULL){
          if(root->opr[0]->decl_node->type!=TYPE_BOOL){
            printf("Error line %d: type should be bool",root->lineno);
        
        exit(-1);
            
            
            
        }

        
          } 
          if(root->opr[0]->decl_node==NULL && (root->opr[0]->nature==NODE_IDENT)){
                printf("Error line %d: undeclared variable",root->lineno);
                exit(-1); 
          }
          if(root->opr[0]->nature==NODE_INTVAL){
            printf("Error line %d: negation of intval not possible  ",root->lineno);
            exit(-1);
          }
          
        
        
        break;

        case(NODE_SLL):
          parcours_arbre(root->opr[0]);
           if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
           
        parcours_arbre(root->opr[1]);

        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }
        }
            
        
        break;

        case(NODE_SRL):
          parcours_arbre(root->opr[0]);
           if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
                if(root->opr[1]->decl_node!=NULL)
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }

        }

        case(NODE_SRA):
         parcours_arbre(root->opr[0]);
          if(root->opr[0]->decl_node!=NULL){
        root->type=root->opr[0]->decl_node->type;
         }
        parcours_arbre(root->opr[1]);
        if(root->opr[0]->decl_node!=NULL && root->opr[1]->decl_node!=NULL ){
        if(root->opr[0]->decl_node->type!=root->opr[1]->decl_node->type){

        printf("Error line %d: types should be identical in node ",root->lineno);
        exit(-1);
            
        }
        

        }

        if(root->opr[0]->decl_node!=NULL){
            if(root->opr[0]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
        }
        else{
            if(root->opr[1]->decl_node!=NULL){
             if(root->opr[1]->decl_node->type!=TYPE_INT){

                     printf("Error line %d: types should be identical in node",root->lineno);
        exit(-1);
            }
            }

        }


        break;

        case(NODE_UMINUS):
          parcours_arbre(root->opr[0]);
          root->type=root->opr[0]->type;
          if(root->opr[0]->decl_node==NULL && root->opr[0]->nature==NODE_IDENT){
            printf("Error line %d: undeclared variable",root->lineno);
                exit(1); 
                }

          if(root->opr[0]->decl_node!=NULL){
          if(root->opr[0]->decl_node->type!=TYPE_INT){
            printf("Error line %d: moins unaire sur entier uniquement ",root->lineno);
            exit(-1);
          }
          } 
          else{
            if(root->opr[0]->type!=TYPE_INT){
            printf("Error line %d: moins unaire sur entier uniquement ",root->lineno);
            exit(-1);

          }
          }
           
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
    printf("\n");
    }
    
