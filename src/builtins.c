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
  unsigned int nr_bytes = 0;
  
  if (first_arg)
  {
    if (first_arg->type == AST_INT)
    {
      char* intstr = calloc(128, sizeof(char));
      sprintf(intstr, "%d", first_arg->int_value);
      instr = intstr;
    }
    else
    if (first_arg->type == AST_ACCESS)
    {
      char* pushstr = as_f(first_arg, list);
      hexstr = pushstr;

      const char* strlenas = "call strlen\n"
                             "popl \%esp\n";

      hexstr = realloc(hexstr, (strlen(hexstr) + strlen(strlenas) + 1) * sizeof(char));
      strcat(hexstr, strlenas);
    }
    else
    if (first_arg->type == AST_VARIABLE)
    {
      char* pushstr = as_f(first_arg, list);
      hexstr = pushstr;
    }
    
    if (instr)
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

  nr_bytes = nr_bytes ? nr_bytes : (nr_chunks * 4);
  char* sizeasstr = (char*) calloc(1, sizeof(char));

  if (nr_bytes)
  {
    const char* size_template = "movl $%d, %%edx\n";
    sizeasstr = realloc(sizeasstr, (strlen(size_template) + 128) * sizeof(char));
    sprintf(sizeasstr, size_template, nr_bytes);
  }
  else
  {
    const char* size_template = "movl %eax, %edx\n";
    sizeasstr = realloc(sizeasstr, (strlen(size_template) + 128) * sizeof(char));
    strcpy(sizeasstr, size_template);
  }
  
  const char* template = 
                         "%s\n" // buffer
                         "movl %%esp, %%ecx\n" // buffer
                         "addl $%d, %%esp\n"
                         "%s\n" // size
                         "movl $4, %%eax\n" // syscall write
                         "movl $1, %%ebx\n" // stdout
                         "int $0x80\n";


  char* asmb = calloc((hexstr ? strlen(hexstr) : 0) + strlen(template) + strlen(sizeasstr) + 1, sizeof(char));
  sprintf(asmb, template, hexstr ? hexstr : "$0", nr_bytes, sizeasstr);
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
