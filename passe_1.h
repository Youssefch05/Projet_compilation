
#ifndef _PASSE_1_
#define _PASSE_1_

#include "defs.h"


void analyse_passe_1(node_t root);
void verif_declaration_unaire(node_t root);
void verif_decla_binaire(node_t root);
void verif_type_unaire(node_t root);
void verif_type_unaire_bool(node_t root);
void verif_condition_bool_opr1(node_t root);
void verif_condition_bool_opr0(node_t root);
void verif_type_binaire_int(node_t root);
void verif_type_binaire_bool(node_t root);


#endif

