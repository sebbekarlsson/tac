#ifndef TAC_BUILTINS_H
#define TAC_BUILTINS_H
#include "visitor.h"

AST_T* fptr_print(visitor_T* visitor, AST_T* node, list_T* list);
void builtins_init(list_T* list);
#endif
