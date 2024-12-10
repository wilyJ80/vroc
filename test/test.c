#include "lexer_test.h"
#include "parser_integration_tests.h"
#include "parser_unit_tests.h"
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

  opRelTest();
  opRelTest2();
  fatorConTest();
  fatorNegFatorTest();
  fatorArrayOutroTest();
  fatorArrayOutroTest2();
  fatorArrayUniTest();
  fatorSingle();
  declVarArrayBadInitCurly();
  // fatorArrayMultTest();

  printf("--- Parser unit tests passed\n");

  progStartKeyword();

  declListVarInvalidType();
  declVarNoId();
  declListVarMulti();
  declListVarMultiFail();
  declVarArrayInvalidSubscript();
  declVarArrayDidntClose();
  declVarBadInit();
  declVarArrayBadInitCurly();
  declVarArrayMultiTooMany();
  declVarArrayInvalidTypeInit();
  declVarArrayMultiInit();
  declVarArrayBadClose();

  declDefProcNoId();
  declDefProcProtNoOpenParen();
  declDefProcProtoInvalidParamType();
  declDefProcProtoNoParamId();
  declDefProcProtoNoValidTokenAfterType();
  declDefProcProtoUnclosedArrayParam();
  declDefProcProtoInvalid2dArrayOpen();
  declDefProcProtoInvalid2dArrayClose();
  declDefProcProtoInvalid3dArray();
  declDefProcProtoMultiParams();
  declDefProcProtoNoParenClose();
  declDefProcProtoTwoProts();

  declDefProcDefDefWorksToo();
  declDefProcDefNoParenOpen();
  declDefProcDefBadParamType();
  declDefProcDefNoParamId();

  printf("--- Parser integration tests passed\n");

  printf(GREEN "All tests OK\n" RESET);

  return EXIT_SUCCESS;
}
