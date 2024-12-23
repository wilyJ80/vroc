#include "lexer_test.h"
#include "new_parser_tests.h"
#include <stdio.h>
#include <stdlib.h>

// ANSI escape codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

int main(void) {
  lexerTest();
  lexerCharconTest();
  lexerCharconTest2();

  printf("--- Lexer tests passed\n");

  declarationsTest();
  protTest();
  defTest();
  defTestWithRepeatedVariableList();
  defTestWithFailedCmd();
  defTestDoesNotGoBackToDeclListVar();
  defTestNoEndpFound();

  printf("--- Parser integration tests passed\n");

  printf(GREEN "All tests OK\n" RESET);

  return EXIT_SUCCESS;
}
