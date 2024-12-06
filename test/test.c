#include "lexer_test.h"
#include "parser_integration_tests.h"
#include "parser_unit_tests.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  lexerTest();
  lexerCharconTest();
  lexerCharconTest2();

  printf("\033[32m--- Lexer tests passed\n\033[0m");

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

  printf("\033[32m--- Parser unit tests passed\n\033[0m");

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

  printf("\033[32m--- Parser integration tests passed\n\033[0m");

  return EXIT_SUCCESS;
}
