#include "include/compiler_errors.h"
#include <stdio.h>

void assert_call_matches_signature(AST_T* call_node, AST_T* func_node)
{
  int expected_nr_args = func_node->children->size;
  int actual_nr_args = call_node->value->children->size;

  if (actual_nr_args < expected_nr_args)
  {
    fprintf(stderr,
      "[Compilation Error]: `%s` expected \033[1;32m%d\033[0m arguments bot got \033[1;31m%d\033[0m.\n",
      func_node->name,
      expected_nr_args,
      actual_nr_args
    );

    exit(1);
  }
}
