#include "include/as_frontend.h"
#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char BOOTSTRAP_TEMPLATE[] =
"print:\n"
" pushl \%ebp\n"
" movl \%esp, \%ebp\n"
" pushl 8(\%esp)\n"
" call strlen\n"
" addl $4, \%esp\n"
" movl 8(\%esp), \%ecx\n" // buffer
" movl \%eax, \%edx\n" // size
" movl $4, \%eax\n" // syscall write
" movl $1, \%ebx\n" // stdout
" movl \%ebp, \%esp\n"
" popl \%ebp\n"
" int $0x80\n"
" ret\n"
"\n"
"return_statement:\n"
" popl \%eax\n"
" movl \%ebp, \%esp\n"
" popl \%ebp\n\n"
" ret\n"
"\n"
" .type strlen, @function\n"
" strlen:\n"
"   pushl %ebp\n"
"   movl %esp, %ebp\n"
"   movl $0, %edi\n"
"   movl 8(%esp), %eax\n"
"   jmp strlenloop\n"
"\n" 
" strlenloop:\n"
"   movb (%eax, %edi, 1), %cl\n"
"   cmpb $0, %cl\n"
"   je strlenend\n"
"   addl $1, %edi\n"
"   jmp strlenloop\n"
"\n"
" strlenend:\n"
"   movl %edi, %eax\n"
"   movl %ebp, %esp\n"
"   popl %ebp\n"
"   ret\n";

static AST_T* var_lookup(list_T* list, const char* name)
{
  for (int i = 0; i < (int) list->size; i++)
  {
    AST_T* child_ast = (AST_T*) list->items[i];

    if (child_ast->type != AST_VARIABLE || !child_ast->name)
      continue;

    if (strcmp(child_ast->name, name) == 0)
      return child_ast;
  }

  return 0;
}

char* as_f_compound(AST_T* ast, list_T* list) {
  char* value = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < ast->children->size; i++)
  {
    AST_T* child_ast = (AST_T*) ast->children->items[i];
    char* next_value = as_f(child_ast, list);
    value = realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
    strcat(value, next_value);
  }

  return value;
}

char* as_f_assignment(AST_T* ast, list_T* list)
{
  char* s = calloc(1, sizeof(char));

  if (ast->value->type == AST_FUNCTION)
  {
    const char* template = ".globl %s\n"
                           "%s:\n"
                           "pushl %%ebp\n"
                           "movl %%esp, %%ebp\n";
    s = realloc(s, (strlen(template) + (strlen(ast->name)*2) + 1) * sizeof(char));
    sprintf(s, template, ast->name, ast->name);

    AST_T* as_val = ast->value;

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
  }

  return s;
}
char* as_f_variable(AST_T* ast, list_T* list) {
  char* s = calloc(1, sizeof(char));

  AST_T* var = var_lookup(list, ast->name);

  if (!var)
  {
    printf("[As Frontend]: `%s` is not defined.\n", var->name);
    exit(1);
  }

  const char* template = "pushl %d(%%esp)\n";
  s = realloc(s, (strlen(template) + 8) * sizeof(char));
  sprintf(s, template, 4 + var->int_value);

  return s;
}

char* as_f_call(AST_T* ast, list_T* list)
{
  char* s = calloc(1, sizeof(char));

  unsigned int i = 0;
  unsigned int next_push_counter = 0;
  int* int_list = calloc(0, sizeof(int));
  size_t int_list_size = 0;

  const char* prefix_template = "subl $%d, %%esp\n";
  char* final_prefix = calloc(0, sizeof(char));
  unsigned int has_final_prefix = 0;
  

  for (; i < ast->value->children->size; i++)
  {
    AST_T* arg = (AST_T*)ast->value->children->items[i];
   
    if (arg->type == AST_STRING) {
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

    if (arg->type == AST_STRING)
    { 
      const char* suffix_template = "movl %%esp, -%d(%%ebp)\n"
                                    "# end of %s\n";
      char* suffix = calloc(strlen(suffix_template) + 128, sizeof(char));
      sprintf(suffix, suffix_template, next_push_counter, arg->string_value);
      s = realloc(s, (strlen(s) + strlen(suffix) + 1) * sizeof(char));
      strcat(s, suffix);
      
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
                         "addl $%d, %%esp\n";
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
  const char* template = "pushl $%d\n";
  char* s = calloc(strlen(template) + 128, sizeof(char));
  sprintf(s, template, ast->int_value);

  return s;
}

char* as_f_string(AST_T* ast, list_T* list)
{
  list_T* chunks = str_to_hex_chunks(ast->string_value);
  unsigned int nr_bytes = ((chunks->size + 1) * 4);
  unsigned int bytes_counter = nr_bytes - 4;
  
  const char* subl_template = "subl $%d, %%esp\n";
  char* sub = calloc(strlen(subl_template) + 128, sizeof(char));
  sprintf(sub, subl_template, nr_bytes);
  
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

  /*const char* finalpushstr = "pushl \%esp\n";

  strpush = realloc(strpush, (strlen(strpush) + strlen(finalpushstr) + 1) * sizeof(char));
  strcat(strpush, finalpushstr);*/

  /*const char* pushsize_template = "pushl $%d\n";
  char* push_size_str = calloc(strlen(pushsize_template) + 128, sizeof(char));
  sprintf(push_size_str, pushsize_template, nr_bytes);
  strpush = realloc(strpush, (strlen(strpush) + strlen(push_size_str) + 1) * sizeof(char));
  strcat(strpush, push_size_str);*/

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

  value = realloc(value, (strlen(value) + strlen(BOOTSTRAP_TEMPLATE) + 1) * sizeof(char));
  strcat(value, BOOTSTRAP_TEMPLATE);

  return value;
}

char* as_f_access(AST_T* ast, list_T* list)
{
  int offset = 12 + (ast->int_value * 4);
  

  const char* template = "# access\n"
                         "pushl %d(%%ebp)\n";

  char* s = calloc(strlen(template) + 128, sizeof(char));
  sprintf(s, template, offset);

  const char* strlenas = "call strlen\n"
                         "pushl %eax\n";

  strcat(s, strlenas);

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
    case AST_ACCESS: next_value = as_f_access(ast, list); break;
    case AST_STATEMENT_RETURN: next_value = as_f_statement_return(ast, list); break;
    default: { printf("[As Frontend]: No frontend for AST of type `%d`\n", ast->type); exit(1); } break;
  }

  value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
  strcat(value, next_value);

  return value;
}
