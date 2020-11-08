#include "../../src/include/tac.h"
#include "../../src/include/io.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_compiler_does_not_segfault()
{
  tac_compile_file("src/input_files/hello_world.tac");
  assert(1);
}

void test_output_hello_world()
{
  tac_compile_file("src/input_files/hello_world.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/input_files/hello_world.tac");
  assert(strcmp(output, expected_output) == 0);
}

int main(int argc, char* argv[])
{
  test_compiler_does_not_segfault();
  test_output_hello_world();


  printf("[Success] All tests completed successfully!\n");
  return 0;
}
