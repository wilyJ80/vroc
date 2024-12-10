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

  printf(GREEN "--- Lexer tests passed\n" RESET);

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

  printf(GREEN "--- Parser unit tests passed\n" RESET);

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

  printf(GREEN "--- Parser integration tests passed\n" RESET);

  return EXIT_SUCCESS;
}
