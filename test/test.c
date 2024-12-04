#include "lexer_test.h"
#include "parser_unit_tests.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  lexerTest();
  lexerCharconTest();
  lexerCharconTest2();

  printf("--- Lexer tests passed\n");

  opRelTest();
  opRelTest2();
  fatorConTest();
  fatorNegFatorTest();
  fatorArrayOutroTest();
  fatorArrayOutroTest2();
  fatorArrayUniTest();
  fatorSingle();
  //fatorArrayMultTest();

  printf("--- Parser tests passed\n");

  return EXIT_SUCCESS;
}
