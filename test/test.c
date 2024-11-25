#include "lexer_test.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  lexerTest();
  lexerCharconTest();
  lexerCharconTest2();

  printf("--- Lexer test passed\n");

  return EXIT_SUCCESS;
}
