#include "include/utils.h"
#include "include/macros.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void assert_not_null(void* x)
{
  if (!!x)
    return;

  printf("[ASSERT FAILED]: %p\n", x);
  exit(1);
}

char* str_to_hex(const char* instr)
{
  unsigned int len = strlen(instr);
  char* hexstr = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < len+1; i++)
  {
    char* newstr = calloc(4, sizeof(char));
    sprintf(newstr, "%x", instr[(len-i)]);
    hexstr = realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
    strcat(hexstr, newstr);
    free(newstr);
  }

  return hexstr;
}

list_T* str_to_hex_chunks(const char* instr)
{
  list_T* list = init_list(sizeof(char*));

  unsigned int i = 0;
  char* tmp = calloc(1, sizeof(char));
  unsigned int len = strlen(instr);
  while (instr[i] != '\0')
  {
    tmp = realloc(tmp, (strlen(tmp) + 2) * sizeof(char));
    strcat(tmp, (char[]){ instr[i], 0 });

    if (((i > 0 && (strlen(tmp) % 4 == 0)) || i >= len-1 ) || instr[i] == '\n' || instr[i] == '\t')
    {
      char* hexstr = str_to_hex(tmp);
      free(tmp);
      list_push(list, hexstr);
      tmp = calloc(1, sizeof(char));
    }
    
    i += 1;
  }

  return list;
}

char* mkstr(const char* str)
{
  char* outstr = (char*) calloc(strlen(str) + 1, sizeof(char));
  strcpy(outstr, str);

  return outstr;
}

char* str_format(char* instr)
{
  char* newstr = calloc(1, sizeof(char));
  unsigned int len = strlen(instr);

  unsigned int i = 0;
  while (instr[i] != '\0' && i < len)
  {
    newstr = realloc(newstr, (strlen(newstr) + 2) * sizeof(char));

    if (instr[i] == '\\')
    {
      char escape = str_to_escape_seq((char[]){ instr[i], instr[MIN(i+1, len)], 0 });
      strcat(newstr, (char[]){escape, 0});
      i += 2;
    }
    else
    {
      strcat(newstr, (char[]){instr[i], 0});
      i += 1;
    }
  }

  return newstr;
}

char str_to_escape_seq(const char* instr)
{
  if (strlen(instr) <= 1) return 0;
  if (instr[0] != '\\') return 0;
  char in_c = instr[1];

  switch (in_c)
  {
    case 'n': return '\n';
    case 't': return '\t';
    case 'r': return '\r';
    case '\\': return in_c;
    default: return in_c;
  }

  return in_c;
}
