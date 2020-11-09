#include "../../src/include/tac.h"
#include "../../src/include/io.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_compiler_does_not_segfault()
{
  tac_compile_file("src/input_files/hello_world.tac");
  assert(1);
  printf("[Success]: test_compiler_does_not_segfault\n");
}

void test_output_hello_world()
{
  tac_compile_file("src/input_files/hello_world.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/hello_world.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_hello_world\n");
}

void test_output_variables()
{
  tac_compile_file("src/input_files/variables.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/variables.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_variables\n");
}

void test_output_arith_add()
{
  tac_compile_file("src/input_files/arith_add.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/arith_add.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_arith_add\n");
}

int main(int argc, char* argv[])
{
  test_compiler_does_not_segfault();
  test_output_hello_world();
  test_output_variables();
  test_output_arith_add();


  printf("[Success]: All tests completed successfully!\n");
  return 0;
}
