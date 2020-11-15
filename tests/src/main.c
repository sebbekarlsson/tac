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
  printf("[success]: test_output_arith_add\n");
}

void test_output_arith_div()
{
  tac_compile_file("src/input_files/arith_div.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/arith_div.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_arith_div\n");
}

void test_output_arith_mul()
{
  tac_compile_file("src/input_files/arith_mul.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/arith_mul.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_arith_mul\n");
}

void test_output_arith_sub()
{
  tac_compile_file("src/input_files/arith_sub.tac");
  char* output = sh("./a.out");
  char* expected_output = tac_read_file("src/output_files/arith_sub.tac");
  assert(strcmp(output, expected_output) == 0);
  printf("[Success]: test_output_arith_sub\n");
}

int main(int argc, char* argv[])
{
  test_compiler_does_not_segfault();
  test_output_hello_world();
  test_output_variables();
  test_output_arith_add();
  test_output_arith_div();
  test_output_arith_mul();


  printf("[Success]: All tests completed successfully!\n");
  return 0;
}
