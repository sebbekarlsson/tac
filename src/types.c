#include "include/types.h"
#include <string.h>


int typename_to_int(const char* name)
{
  if (strcmp(name, "int") == 0) return DATA_TYPE_INT;

  return DATA_TYPE_UNKNOWN;
}
