#ifndef _MINICCUTILS_H_
#define _MINICCUTILS_H_

#include <stdint.h>
#include <stdbool.h>

#include "../defs.h"



/* For checking the tree constructed */

bool check_program_tree(node_t n);


/* Context related functions */

typedef struct _context_s context_s;
typedef context_s * context_t;

context_t create_context();
void free_context(context_t context);
bool context_add_element(context_t context, char * idf, void * data);
void * get_data(context_t context, char * idf);


/* Environment related functions */

void push_global_context();
void push_context();
void pop_context();
void * get_decl_node(char * ident);
int32_t env_add_element(char * ident, void * node);
void reset_env_current_offset();
int32_t get_env_current_offset();
int32_t add_string(char * str);
int32_t get_global_strings_number();
char * get_global_string(int32_t index);
void free_global_strings();


/* Register allocation related functions */

void push_temporary(int32_t reg);
void pop_temporary(int32_t reg);
bool reg_available();
int32_t get_current_reg();
int32_t get_restore_reg();
void allocate_reg();
void release_reg();
int32_t get_new_label();
void set_temporary_start_offset(int32_t offset);
void set_max_registers(int32_t n);
void reset_temporary_max_offset();
int32_t get_temporary_max_offset();
int32_t get_temporary_curr_offset(); // for debug


/* Program creation related functions */

void data_sec_inst_create();
void text_sec_inst_create();
void word_inst_create(char * label, int32_t init_value);
void asciiz_inst_create(char * label_str, char * str);
void label_inst_create(int32_t label);
void label_str_inst_create(char * label);
void comment_inst_create(char * comment);
void lui_inst_create(int32_t r_dest, int32_t imm);
void addu_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void subu_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void slt_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void sltu_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void and_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void or_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void xor_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void nor_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void mult_inst_create(int32_t r_src_1, int32_t r_src_2);
void div_inst_create(int32_t r_src_1, int32_t r_src_2);
void sllv_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void srav_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void srlv_inst_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void addiu_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void andi_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void ori_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void xori_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void slti_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void sltiu_inst_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void lw_inst_create(int32_t r_dest, int32_t imm, int32_t r_src_1);
void sw_inst_create(int32_t r_src_1, int32_t imm, int32_t r_src_2);
void beq_inst_create(int32_t r_src_1, int32_t r_src_2, int32_t label);
void bne_inst_create(int32_t r_src_1, int32_t r_src_2, int32_t label);
void mflo_inst_create(int32_t r_dest);
void mfhi_inst_create(int32_t r_dest);
void j_inst_create(int32_t label);
void teq_inst_create(int32_t r_src_1, int32_t r_src_2);
void syscall_inst_create();
void stack_allocation_inst_create();
void stack_deallocation_inst_create(int32_t val);

void create_program();
void free_program();
void dump_mips_program(char * filename);




#endif

