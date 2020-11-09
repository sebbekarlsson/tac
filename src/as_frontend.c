#include "include/as_frontend.h"
#include "include/utils.h"
#include "include/token.h"
#include "include/bootstrap.h"
#include "include/macros.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* as_f_compound(AST_T* ast, list_T* list) {
  int index = ast->stack_frame->stack->size * 4;
  const char* template = "subl $%d, %%esp\n";
  char* value = calloc(strlen(template) + 128, sizeof(char));
  sprintf(value, template, index);

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
  AST_T* parent = list->size ? (AST_T*)list->items[list->size-1] : (AST_T*)0;
  if (!parent) return 0;

  list->items[list->size-1] = 0;
  list->size -= 1;

  char* name = parent->name;

  const char* template = ".globl %s\n"
                           "%s:\n"
                           "pushl %%ebp\n"
                           "movl %%esp, %%ebp\n";
  char* s = calloc((strlen(template) + (strlen(name)*2) + 1), sizeof(char));
  sprintf(s, template, name, name);

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
    list_push(list, arg_variable);
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

  /*const char* subl_template = "subl $4, %esp\n";
  char* subl = calloc(strlen(subl_template) + 1, sizeof(char));
  strcpy(subl, subl_template);
 
  s = realloc(s, (strlen(s) + strlen(subl) + 1) * sizeof(char));
  strcat(s, subl);
  free(subl);*/

  if (ast->value->type == AST_FUNCTION)
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
    const char* mo_template = "# assign (%s)\n"
                              "movl $-%d, %%edi\n"
                              "movb %%cl, (%%ebp, %%edi, 1)\n";
    char* mo = calloc(strlen(mo_template) + 128, sizeof(char));
    sprintf(mo, mo_template, ast->name, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo); 
  }
  if (ast->value->type == AST_CALL)
  {
    const char* mo_template = "movl %%eax, -%d(%%ebp)\n";
    char* mo = calloc(strlen(mo_template) + 128, sizeof(char));
    sprintf(mo, mo_template, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
  }
  else
  { 
    const char* mo_template = "movl %%esp, -%d(%%ebp)\n";
    char* mo = calloc(strlen(mo_template) + 128, sizeof(char));
    sprintf(mo, mo_template, id);
    s = realloc(s, (strlen(s) + strlen(mo) + 1) * sizeof(char));
    strcat(s, mo);
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

  unsigned int next_push_counter = ((ast->stack_frame->stack->size - ast->value->children->size) - 1) * 4;
  int* int_list = calloc(0, sizeof(int));
  size_t int_list_size = 0;

  const char* prefix_template = "subl $%d, %%esp\n";
  char* final_prefix = calloc(0, sizeof(char));
  unsigned int has_final_prefix = 0;
  

  unsigned int i = ast->value->children->size;
  for (; i > 0; i--)
  {
    AST_T* arg = (AST_T*)ast->value->children->items[i-1];
   
    if (arg->type == AST_STRING || arg->type == AST_INT) {
      next_push_counter += 4;

      char* prefix = calloc(strlen(prefix_template) + 128, sizeof(char));
      sprintf(prefix, prefix_template, 4);
      final_prefix = realloc(final_prefix, (strlen(final_prefix) + strlen(prefix) + 1) * sizeof(char));
      strcat(final_prefix, prefix);
      has_final_prefix = 1;
      free(prefix);
    }

    char* arg_s = as_f(arg, list);

    s = realloc(s, (strlen(s) + strlen(arg_s) + 1) * sizeof(char));
    strcat(s, arg_s);

    if (arg->type == AST_STRING || arg->type == AST_INT)
    { 
      /*const char* suffix_template = "movl %%esp, -%d(%%ebp)\n"
                                    "# end of %s\n";
      char* suffix = calloc(strlen(suffix_template) + 128, sizeof(char));
      sprintf(suffix, suffix_template, next_push_counter, arg->string_value);
      s = realloc(s, (strlen(s) + strlen(suffix) + 1) * sizeof(char));
      strcat(s, suffix);*/
      
      int_list_size += 1;
      int_list = realloc(int_list, int_list_size * sizeof(int));
      int_list[int_list_size-1] = next_push_counter;
    }
  }

  for (unsigned int i = 0; i < int_list_size; i++)
  {
    const char* push_template = "pushl -%d(%%ebp)\n";
    char* push = calloc(strlen(push_template) * 128, sizeof(char));
    sprintf(push, push_template, int_list[i]);
    s = realloc(s, (strlen(s) + strlen(push) + 1) * sizeof(char));
    strcat(s, push);
    free(push);
  }

  if (int_list && int_list_size)
    free(int_list);

  int addl_size = i * 4;

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
  const char* template = "# integer\n"
                         "pushl $%d\n"
                         "movb (%%esp), %%cl\n"
                         "movl $%d, -%d(%%ebp)\n";
  char* s = calloc(strlen(template) + 128, sizeof(char));
  sprintf(s, template, ast->int_value, ast->int_value, index);

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

  const char* final_template = "movl %%esp, -%d(%%ebp)\n";
  char* final = calloc(strlen(final_template) + 128, sizeof(char));
  sprintf(final, final_template, index + 4);

  /*const char* finalpushstr = "pushl \%esp\n";

  strpush = realloc(strpush, (strlen(strpush) + strlen(finalpushstr) + 1) * sizeof(char));
  strcat(strpush, finalpushstr);*/

  /*const char* pushsize_template = "pushl $%d\n";
  char* push_size_str = calloc(strlen(pushsize_template) + 128, sizeof(char));
  sprintf(push_size_str, pushsize_template, nr_bytes);
  strpush = realloc(strpush, (strlen(strpush) + strlen(push_size_str) + 1) * sizeof(char));
  strcat(strpush, push_size_str);*/

  strpush = realloc(strpush, (strlen(strpush) + strlen(final) + 1) * sizeof(char));
  strcat(strpush, final);

  return strpush;
}

char* as_f_root(AST_T* ast, list_T* list)
{
  const char* section_text = ".section .text\n"
                             ".globl _start\n"
                             "_start:\n"
                             "movl \%esp, \%ebp\n"
                             "call main\n"
                             "movl \%eax, \%ebx\n"
                             "movl $1, \%eax\n"
                             "int $0x80\n\n";
  char* value = (char*) calloc((strlen(section_text) + 128), sizeof(char));
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
  int offset = 4 + (ast->id * 4);
  

  const char* template = "# access\n"
                         "movl $%d, %%edi\n"
                         "leal (%%ebp, %%edi, 1), %%eax\n"
                         "pushl %d(%%eax)\n";

  char* s = calloc(strlen(template) + 128, sizeof(char));
  sprintf(s, template, offset, MAX(4, (ast->int_value + 1) * 4));

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

  if (ast->op == TOKEN_PLUS)
  {
    value =  "# addition\n"
             "popl %eax\n"
             "addl (%esp), %eax\n"
             "addl $4, %esp\n"
             "pushl %eax\n"
             "movb (%esp), %cl\n"; 
  }
  else
  if (ast->op == TOKEN_MINUS)
  {
    value = "# subtraction\n"
            "popl %eax\n"
            "subl (%esp), %eax\n"
            "addl $4, %esp\n"
            "pushl %eax\n"
            "movb (%esp), %cl\n";
  }
  else
  if (ast->op == TOKEN_MUL)
  {
    value =  "# multiplication\n"
             "popl %eax\n"
             "imull (%esp), %eax\n"
             "addl $4, %esp\n"
             "pushl %eax\n"
             "movb (%esp), %cl\n";
  }
  else
  if (ast->op == TOKEN_DIV)
  {
    value =  "# division\n"
             "popl %eax\n"
             "popl %ecx\n"
             "div %ecx\n"
             "addl $4, %esp\n"
             "pushl %eax\n"
             "movb (%esp), %cl\n";
  }

  if (value)
  {
    s = realloc(s, (strlen(s) + strlen(value) + 1) * sizeof(char));
    strcat(s, value);
  }

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
