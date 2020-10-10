#ifndef TAC_AST_H
#define TAC_AST_H
#include "list.h"

typedef struct AST_STRUCT
{
  enum
  {
    AST_COMPOUND,
    AST_FUNCTION,
    AST_CALL,
    AST_ASSIGNMENT,
    AST_DEFINITION_TYPE,
    AST_VARIABLE,
    AST_STATEMENT,
    AST_ACCESS,
    AST_INT,
    AST_NOOP,
  } type;
  
  list_T* children;
  char* name;
  struct AST_STRUCT* value;
  int int_value;
  int data_type;
} AST_T;

AST_T* init_ast(int type);
#endif
