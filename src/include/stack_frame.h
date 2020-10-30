#ifndef TAC_STACK_FRAME_H
#define TAC_STACK_FRAME_H
#include "list.h"
typedef struct STACK_FRAME_STRUCT
{
  list_T* stack;
} stack_frame_T;

stack_frame_T* init_stack_frame();
#endif
