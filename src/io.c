#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* tac_read_file(const char* filename)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("Could not read file `%s`\n", filename);
    exit(1);
  }

  char* buffer = (char*) calloc(1, sizeof(char));
  buffer[0] = '\0';

  while ((read = getline(&line, &len, fp)) != -1) {
    buffer = (char*) realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
    strcat(buffer, line);
  }

  fclose(fp);
  if (line)
    free(line);

  return buffer;
}

void tac_write_file(const char* filename, char* outbuffer)
{
  FILE * fp;

  fp = fopen(filename, "wb");
  if (fp == NULL)
  {
    printf("Could not open file for writing `%s`\n", filename);
    exit(1);
  }

  fputs(outbuffer, fp);

  fclose(fp);
}
