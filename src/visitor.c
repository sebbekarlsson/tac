#include "include/visitor.h"
#include "include/builtins.h"
#include <stdio.h>
#include <string.h>

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

visitor_T* init_visitor()
{
  visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
  visitor->object = init_ast(AST_COMPOUND);

  builtins_init(visitor->object->children);

  return visitor;
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, list_T* list)
{
  switch (node->type)
  {
    case AST_COMPOUND: return visitor_visit_compound(visitor, node, list); break;
    case AST_ASSIGNMENT: return visitor_visit_assignment(visitor, node, list); break;
    case AST_VARIABLE: return visitor_visit_variable(visitor, node, list); break;
    case AST_CALL: return visitor_visit_call(visitor, node, list); break;
    case AST_INT: return visitor_visit_int(visitor, node, list); break;
    case AST_ACCESS: return visitor_visit_access(visitor, node, list); break;
    case AST_FUNCTION: return visitor_visit_function(visitor, node, list); break;
    case AST_STRING: return visitor_visit_string(visitor, node, list); break;
    default: { printf("[Visitor]: Don't know how to handle AST of type `%d`\n", node->type); exit(1); } break;
  }

  return node;
}

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* compound = init_ast(AST_COMPOUND);

  for (unsigned int i = 0; i < node->children->size; i++)
  {
    AST_T* x = visitor_visit(visitor, (AST_T*) node->children->items[i], list);
    list_push(compound->children, x);
  }

  return compound;
}

AST_T* visitor_visit_assignment(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* new_var = init_ast(AST_ASSIGNMENT);
  new_var->name = node->name;
  new_var->value = visitor_visit(visitor, node->value, list);
  list_push(list, new_var);

  return new_var;
}

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* var = var_lookup(visitor->object->children, node->name);

  if (var)
    return var;

  return node;
}

AST_T* visitor_visit_function(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* func = init_ast(AST_FUNCTION);
  func->children = init_list(sizeof(struct AST_STRUCT*));
  func->value = visitor_visit(visitor, node->value, node->children);

  for (unsigned int i = 0; i < node->children->size; i++)
    list_push(func->children, (AST_T*) visitor_visit(visitor, (AST_T*) node->children->items[i], list));

  return func;
}

AST_T* visitor_visit_call(visitor_T* visitor, AST_T* node, list_T* list)
{
  AST_T* var = var_lookup(visitor->object->children, node->name);

  list_T* new_args = init_list(sizeof(struct AST_STRUCT*));

  for (unsigned int i = 0; i < node->value->children->size; i++)
  {
    list_push(new_args, visitor_visit(visitor, (AST_T*)node->value->children->items[i], list));
  }

  if (var)
  {
    if (var->fptr)
    {
      AST_T* x = visitor_visit(visitor, var->fptr(visitor, var, new_args), list);
      return x;
    }
  }

  return node;
}

AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node, list_T* list)
{
  return node;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node, list_T* list)
{
  return node;
}

AST_T* visitor_visit_access(visitor_T* visitor, AST_T* node, list_T* list)
{
  int id = 0;
  for (unsigned int i = 0; i < list->size; i++)
  {
    if (strcmp(((AST_T*)list->items[i])->name, node->name) == 0)
    {
      id = i;
      break;
    }
  }

  node->id = id;

  return node;
}
