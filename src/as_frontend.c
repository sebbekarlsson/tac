#include "include/as_frontend.h"
#include "include/utils.h"
#include "include/token.h"
#include "include/macros.h"

// asm
#include "include/asm/bootstrap.h"
#include "include/asm/root.h"
#include "include/asm/add.h"
#include "include/asm/mul.h"
#include "include/asm/div.h"
#include "include/asm/sub.h"

#include "include/asm/templates/assign_int.h"
#include "include/asm/templates/assign_call.h"
#include "include/asm/templates/assign_binop.h"
#include "include/asm/templates/assign_default.h"
#include "include/asm/templates/function_begin.h"
#include "include/asm/templates/int.h"
#include "include/asm/templates/access.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* as_f_compound(AST_T* ast, list_T* list) {
  const char* template = "# compound (%p)\n";
  char* value = calloc(strlen(template) + 128, sizeof(char));
  sprintf(value, template, ast);

  for (unsigned int i = 0; i < ast->children->size; i++)
  {
    AST_T* child_ast = (AST_T*) ast->children->items[i];
    char* next_value = as_f(child_ast, list);
    value = realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
    strcat(value, next_value);
  }

  return value;
}

char* as_f_function(AST_T* ast, list_T* list)
{
  char* name = ast->name;
  int index = ast->stack_frame->stack->size * 4;

  char* s = calloc((src_asm_templates_function_begin_asm_len + (strlen(name)*2) + 1), sizeof(char));
  sprintf(s, (char*) src_asm_templates_function_begin_asm, name, index);

  if (ast->stack_frame->stack->size)
  {
    const char* subl_template = "subl $%d, %%esp\n";
    char* subl = calloc(strlen(subl_template) + 128, sizeof(char));
    sprintf(subl, subl_template, (1 + ast->stack_frame->stack->size) * 4);
   
    s = realloc(s, (strlen(s) + strlen(subl) + 1) * sizeof(char));
    strcat(s, subl);
    free(subl);
  }

  AST_T* as_val = ast;

  for (unsigned int i = 0; i < as_val->children->size; i++)
  {
    AST_T* farg = (AST_T*) as_val->children->items[i];
    AST_T* arg_variable = init_ast(AST_VARIABLE);
    arg_variable->name = farg->name;
    arg_variable->int_value = (4 * as_val->children->size) - (i*4);
  }
  
  char* as_val_val = as_f(as_val->value, list);

  s = realloc(s, (strlen(s) + strlen(as_val_val) + 1) * sizeof(char));
  strcat(s, as_val_val);
  free(as_val_val);

  return s;
}

char* as_f_assignment(AST_T* ast, list_T* list)
{
  int id = (ast->stack_index*4);

  char* s = calloc(1, sizeof(char));
  
  list_push(list, ast);

  char* value_as = as_f(ast->value, list);

  if (value_as)
  {
    s = realloc(s, (strlen(s) + strlen(value_as) + 1) * sizeof(char));
    strcat(s, value_as);
    free(value_as);
  }

  if (ast->dtype == DATA_TYPE_INT)
  {
    char* mo = calloc(src_asm_templates_assign_int_asm_len + 128, sizeof(char));
    sprintf(mo, (char*)src_asm_templates_assign_int_asm, ast->name, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
    free(mo); 
  }
  else
  if (ast->value->type == AST_CALL)
  {
    char* mo = calloc(src_asm_templates_assign_call_asm_len + 128, sizeof(char));
    sprintf(mo, (char*) src_asm_templates_assign_call_asm, id, ast->value->value->children->size * 4);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
    free(mo);
  }
  else if (ast->value->type == AST_BINOP)
  { 
    char* mo = calloc(src_asm_templates_assign_binop_asm_len + 128, sizeof(char));
    sprintf(mo, (char*) src_asm_templates_assign_binop_asm, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
    free(mo);
  }
  else
  { 
    char* mo = calloc(src_asm_templates_assign_default_asm_len + 128, sizeof(char));
    sprintf(mo, (char*) src_asm_templates_assign_default_asm, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
    free(mo);
  }

  return s;
}

char* as_f_variable(AST_T* ast, list_T* list) {
  char* s = calloc(1, sizeof(char));

  const char* template = "# variable (%s)\n"
                         "pushl %d(%%ebp)\n";
  int id = ((ast->stack_index)*4);
  s = realloc(s, (strlen(template) + 8) * sizeof(char));
  sprintf(s, template, ast->name, id);

  return s;
}

char* as_f_call(AST_T* ast, list_T* list)
{
  char* s = calloc(1, sizeof(char));

  char* final_prefix = calloc(0, sizeof(char));
  unsigned int has_final_prefix = 0;
  
  unsigned int i = ast->value->children->size;
  for (i = 0; i < ast->value->children->size; i++)
  {
    AST_T* arg = (AST_T*)ast->value->children->items[i];
    char* arg_s = as_f(arg, list);

    s = realloc(s, (strlen(s) + strlen(arg_s) + 1) * sizeof(char));
    strcat(s, arg_s);
  }

  for (i = 0; i < ast->value->children->size; i++)
  {
    AST_T* arg = (AST_T*)ast->value->children->items[i];

    const char* push_template ="# call arg\n" 
                               "pushl %d(%%ebp)\n";
    char* push = calloc(strlen(push_template) * 128, sizeof(char));
    sprintf(push, push_template, (arg->stack_index + (arg->type == AST_STRING ? 1 : 0)) * 4);
    s = realloc(s, (strlen(s) + strlen(push) + 1) * sizeof(char));
    strcat(s, push);
    free(push);
  }

  int addl_size = i * 4;

  if (list->size)
  {
    if (((AST_T*)list->items[0])->type == AST_ASSIGNMENT)
      addl_size = 0;
  }

  const char* template = "call %s\n"
                         "addl $%d, %%esp\n"
                         "pushl %%eax\n";
  char* ret_s = calloc(strlen(template) + 128, sizeof(char));
  sprintf(ret_s, template, ast->name, addl_size);
  s = realloc(s, (strlen(s) + strlen(ret_s) + 1) * sizeof(char));
  strcat(s, ret_s);
  free(ret_s);

  char* final_str = calloc(strlen(s) + strlen(final_prefix) + 1, sizeof(char));
  strcat(final_str, final_prefix);
  strcat(final_str, s);

  free(s);

  if (has_final_prefix)
    free(final_prefix);

  return final_str;
}

char* as_f_statement_return(AST_T* ast, list_T* list)
{
  char* s = calloc(1, sizeof(char));

  const char* template = "%s\n"
                         "jmp return_statement\n";
  char* value_s = as_f(ast->value, list);
  char* ret_s = calloc(strlen(template) + strlen(value_s) + 128, sizeof(char));
  sprintf(ret_s, template, value_s);
  s = realloc(s, (strlen(ret_s) + 1) * sizeof(char));
  strcat(s, ret_s);
  free(ret_s);

  return s;
}

char* as_f_int(AST_T* ast, list_T* list)
{
  int index = ast->stack_index * 4;
  char* s = calloc(src_asm_templates_int_asm_len + 128, sizeof(char));
  sprintf(s, (char*) src_asm_templates_int_asm, ast->int_value, ast->int_value, index);

  return s;
}

char* as_f_string(AST_T* ast, list_T* list)
{
  list_T* chunks = str_to_hex_chunks(ast->string_value);
  unsigned int nr_bytes = ((chunks->size + 1) * 4);
  unsigned int bytes_counter = nr_bytes - 4;
  int index = ast->stack_index * 4;
  
  const char* subl_template = "# %s\n"
                              "subl $%d, %%esp\n";
  char* sub = calloc(strlen(subl_template) + 128, sizeof(char));
  sprintf(sub, subl_template, ast->string_value, nr_bytes);
  
  char* strpush = calloc(strlen(sub) + 1, sizeof(char));
  strcat(strpush, sub);

  const char* zero_push_template = "movl $0x0, %d(%%esp)\n";
  char* zero_push = calloc(strlen(zero_push_template) + 128, sizeof(char));
  sprintf(zero_push, zero_push_template, bytes_counter);
  strpush = realloc(strpush, (strlen(zero_push) + strlen(strpush) + 1) * sizeof(char));
  strcat(strpush, zero_push);
  
  bytes_counter -= 4;

  const char* pushtemplate = "movl $0x%s, %d(%%esp)\n";
  
  for (unsigned int i = 0; i < chunks->size; i++)
  {
    char* pushhex = (char*) chunks->items[(chunks->size - i)-1];
    char* push = calloc(strlen(pushhex) + strlen(pushtemplate) + 1, sizeof(char));
    sprintf(push, pushtemplate, pushhex, bytes_counter);
    strpush = realloc(strpush, (strlen(strpush) + strlen(push) + 1) * sizeof(char));
    strcat(strpush, push);
    free(pushhex);
    free(push);
    bytes_counter -= 4;
  }

  const char* final_template = "movl %%esp, %d(%%ebp)\n";
  char* final = calloc(strlen(final_template) + 128, sizeof(char));
  sprintf(final, final_template, index + 4);

  strpush = realloc(strpush, (strlen(strpush) + strlen(final) + 1) * sizeof(char));
  strcat(strpush, final);

  return strpush;
}

char* as_f_root(AST_T* ast, list_T* list)
{
  const char* section_text = (char*) src_asm_root_asm;
  char* value = (char*) calloc((src_asm_root_asm_len + 128), sizeof(char));
  strcpy(value, section_text);

  char* next_value = as_f(ast, list);
  value = (char*) realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
  strcat(value, next_value);

  value = realloc(value, (strlen(value) + src_asm_bootstrap_asm_len + 1) * sizeof(char));
  strcat(value, (char*) src_asm_bootstrap_asm);

  return value;
}

char* as_f_access(AST_T* ast, list_T* list)
{
  int offset = ((ast->stack_index * -1) * 4) - 4;
  int array_offset = MAX(4, (ast->int_value + 1) * 4);
  
  char* s = calloc(src_asm_templates_access_asm_len + 128, sizeof(char));
  sprintf(s, (char*)src_asm_templates_access_asm, offset, array_offset, array_offset, ast->stack_index * 4);

  return s;
}

char* as_f_binop(AST_T* ast, list_T* list)
{
  char* s = calloc(1, sizeof(char));

  char* left_f_str = as_f(ast->left, list);
  char* right_f_str = as_f(ast->right, list);
  s = realloc(s, (strlen(left_f_str) + strlen(right_f_str) + 1) * sizeof(char));
  strcat(s, right_f_str);
  strcat(s, left_f_str);

  char* value = 0;

  switch (ast->op)
  {
    case TOKEN_PLUS: value = (char*) src_asm_add_asm; break;
    case TOKEN_MINUS: value = (char*) src_asm_sub_asm; break;
    case TOKEN_MUL: value = (char*) src_asm_mul_asm; break;
    case TOKEN_DIV: value = (char*) src_asm_div_asm; break;
    default: { printf("[As Frontend]: No frontend binop `%d`\n", ast->op); exit(1); } break;
  }

  s = realloc(s, (strlen(s) + strlen(value) + 1) * sizeof(char));
  strcat(s, value);

  free(left_f_str);
  free(right_f_str);

  return s;
}

char* as_f(AST_T* ast, list_T* list)
{
  char* value = calloc(1, sizeof(char));
  char* next_value = 0;

  switch (ast->type)
  {
    case AST_COMPOUND: next_value = as_f_compound(ast, list); break;
    case AST_ASSIGNMENT: next_value = as_f_assignment(ast, list); break;
    case AST_VARIABLE: next_value = as_f_variable(ast, list); break;
    case AST_CALL: next_value = as_f_call(ast, list); break;
    case AST_INT: next_value = as_f_int(ast, list); break;
    case AST_STRING: next_value = as_f_string(ast, list); break;
    case AST_BINOP: next_value = as_f_binop(ast, list); break;
    case AST_ACCESS: next_value = as_f_access(ast, list); break;
    case AST_STATEMENT_RETURN: next_value = as_f_statement_return(ast, list); break;
    case AST_FUNCTION: next_value = as_f_function(ast, list); break;
    default: { printf("[As Frontend]: No frontend for AST of type `%d`\n", ast->type); exit(1); } break;
  }

  value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
  strcat(value, next_value);

  return value;
}
