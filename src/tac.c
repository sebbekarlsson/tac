#include "include/tac.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/visitor.h"
#include "include/as_frontend.h"
#include <string.h>
#include <stdlib.h>


static char* sh(const char* cmd)
{
  char* output = (char*) calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  fp = popen(cmd, "r");

  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char*) realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
    strcat(output, path);
  }

  pclose(fp);

  return output;
}

void tac_compile(char* src)
{
  lexer_T* lexer = init_lexer(src);
  parser_T* parser = init_parser(lexer);
  AST_T* root = parser_parse(parser);

  visitor_T* visitor = init_visitor();
  AST_T* optimized_root = visitor_visit(visitor, root, init_list(sizeof(struct AST_STRUCT*)), init_stack_frame());
  
  char* s = as_f_root(optimized_root, init_list(sizeof(struct AST_STRUCT*)));

  tac_write_file("a.s", s);
  tac_write_file("a.s.txt", s);
  sh("as --32 a.s -o a.o");
  sh("ld a.o -o a.out -m elf_i386");
}

void tac_compile_file(const char* filename)
{
  char* src = tac_read_file(filename);
  tac_compile(src);
  free(src);
}
