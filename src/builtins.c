#include "include/builtins.h"
#include "include/utils.h"
#include "include/as_frontend.h"
#include <string.h>
#include <stdio.h>

AST_T* fptr_print(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* ast = init_ast(AST_STRING);

  AST_T* first_arg = list->size ? (AST_T*) (AST_T*)list->items[0] : (AST_T*)0;
  char* instr = first_arg ? first_arg->string_value : 0;
  char* hexstr = 0;
  unsigned int nr_chunks = 0;
  
  if (first_arg)
  {
    if (first_arg->type == AST_INT)
    {
      instr = calloc(128, sizeof(char));
      sprintf(instr, "%d", first_arg->int_value);
    }
    else
    if (first_arg->type == AST_ACCESS)
    {
      char* pushstr = as_f(first_arg, list);
      hexstr = pushstr;
    }
    else
    if (first_arg->type == AST_VARIABLE)
    {
      return first_arg;
    }
    else
    {
      list_T* chunks = str_to_hex_chunks(instr);
      nr_chunks = chunks->size;

      char* strpush = calloc(1, sizeof(char));
      const char* pushtemplate = "pushl $0x%s\n";

      for (unsigned int i = 0; i < chunks->size; i++)
      {
        char* pushhex = (char*) chunks->items[(chunks->size - i)-1];
        char* push = calloc(strlen(pushhex) + strlen(pushtemplate) + 1, sizeof(char));
        sprintf(push, pushtemplate, pushhex);
        strpush = realloc(strpush, (strlen(strpush) + strlen(push) + 1) * sizeof(char));
        strcat(strpush, push);
        free(pushhex);
        free(push);
      }
      
      hexstr = strpush;
    }
  }


  const char* template = "movl $4, %%eax\n" // syscall write
                         "movl $1, %%ebx\n" // stdout
                         "%s\n" // buffer
                         "movl %%esp, %%ecx\n" // buffer
                         "addl $%d, %%esp\n"
                         "movl $%d, %%edx\n" // size
                         "int $0x80\n";

  unsigned int nr_bytes = nr_chunks * 4;

  char* asmb = calloc((hexstr ? strlen(hexstr) : 0) + strlen(template) + 1, sizeof(char));
  sprintf(asmb, template, hexstr ? hexstr : "$0", nr_bytes, nr_bytes);
  ast->string_value = asmb;
  free(hexstr);

  return ast;
}

void builtins_register_fptr(list_T* list, const char* name, AST_T* (*fptr)(visitor_T* visitor, AST_T* node, list_T* list))
{
  AST_T* fptr_print_var = init_ast(AST_VARIABLE);
  fptr_print_var->name = mkstr(name);
  fptr_print_var->fptr = fptr;

  list_push(list, fptr_print_var);
}

void builtins_init(list_T* list)
{
  builtins_register_fptr(list, "print", fptr_print);
}
