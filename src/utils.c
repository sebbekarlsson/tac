#include "include/utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char* str_to_hex(const char* instr)
{
  unsigned int len = strlen(instr);
  char* hexstr = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < len+1; i++)
  {
    char* newstr = calloc(4, sizeof(char));
    sprintf(newstr, "%x", instr[len-i]);
    hexstr = realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
    strcat(hexstr, newstr);
    free(newstr);
  }

  return hexstr;
}

char** str_to_hex_chunks(const char* instr, int* nr_chunks)
{
  unsigned int len = strlen(instr);
  unsigned int nrchunks = (len / 4) + 1;
  int* nrptr = nr_chunks;
  *nrptr = nrchunks;

  char** strlist = calloc(nrchunks * 5, sizeof(char));

  for (unsigned int i = 0; i < (len/4)+1; i++)
  {
    char* chunkstr = mkstr(instr + ((i)*4));
    chunkstr = realloc(chunkstr, 4);
    chunkstr[4] = 0;
    char* hexstr = str_to_hex(chunkstr);

    strlist[i] = hexstr;
  }

  return strlist;
}

char* mkstr(const char* str)
{
  char* outstr = (char*) calloc(strlen(str) + 1, sizeof(char));
  strcpy(outstr, str);

  return outstr;
}
