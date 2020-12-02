#include "include/tac.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr,"\033[1;31mPlease specify input file.\033[0m\n");
    return 1;
  }

  tac_compile_file(argv[1]);

  return 0;
}
