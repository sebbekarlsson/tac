#include "include/compiler_errors.h"
#include <stdio.h>

void assert_call_matches_signature(AST_T* call_node, AST_T* func_node)
{
  int expected_nr_args = func_node->children->size;
  int actual_nr_args = call_node->value->children->size;

  if (actual_nr_args < expected_nr_args)
  {
    printf(
      "[Compilation Error]: `%s` expected %d arguments bot got %d.\n",
      func_node->name,
      expected_nr_args,
      actual_nr_args
    );

    exit(1);
  }
}
