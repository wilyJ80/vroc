#include "parser_integration_tests.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// switch this on to output all syntax errors
#define SHOW_ERRORS false

enum SYNTAX_ERROR setupError(const char *mockData) {
  FILE *mockFile = fmemopen((void *)mockData, strlen(mockData), "r");

  if (mockFile == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  // for integration tests, prog itself needs a previously initialized parser
  // with a token too
  struct Token token = lexerGetNextChar(mockFile, lineCount);
  struct Parser parser = {
      .fd = mockFile, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  if (SHOW_ERRORS) {
    printSyntaxError(error, parser.lineCount);
  }
  return error;
}

void progStartKeyword() {
  const char *mock_data = "==\n";
  enum SYNTAX_ERROR error = setupError("==\n");
  assert(error == INVALID_PROG_START_KEYWORD);
}

void declListVarInvalidType() {
  enum SYNTAX_ERROR error = setupError("const string\n");
  assert(error == INVALID_TYPE);
}

void declVarNoId() {
  enum SYNTAX_ERROR error = setupError("const int 5\n");
  assert(error == NO_VAR_ID);
}

void declVarArrayInvalidSubscript() {
  enum SYNTAX_ERROR error = setupError("const int i[2.2\n");
  assert(error == INVALID_ARRAY_SUBSCRIPT_DEC);
}

void declVarArrayDidntClose() {
  enum SYNTAX_ERROR error = setupError("const int i[2[\n");
  assert(error == INVALID_ARRAY_BRACKET_DEC_CLOSE);
}

void declVarBadInit() {
  enum SYNTAX_ERROR error = setupError("const int i = i\n");
  assert(error == INVALID_VAR_TYPE_INIT);
}

void declListVarMulti() {
  enum SYNTAX_ERROR error = setupError("const int i = 1, a = 2\n");
  assert(error == NO_ERROR);
}

void declListVarMultiFail() {
  enum SYNTAX_ERROR error = setupError("const int i = 1, a = a\n");
  assert(error == INVALID_VAR_TYPE_INIT);
}

void declVarArrayMultiTooMany() {
  enum SYNTAX_ERROR error = setupError("int i[2][3][4]\n");
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declVarArrayBadInitCurly() {
  enum SYNTAX_ERROR error = setupError("const int i[2] = [1\n");
  assert(error == INVALID_ARRAY_INIT_CURLY_OPEN);
}

void declVarArrayInvalidTypeInit() {
  enum SYNTAX_ERROR error = setupError("const int i[2] = {id\n");
  assert(error == INVALID_ARRAY_TYPE_INIT);
}

void declVarArrayMultiInit() {
  enum SYNTAX_ERROR error = setupError("int i[4] = {3, 4, 4, 4}\n");
  assert(error == NO_ERROR);
}

void declVarArrayBadClose() {
  enum SYNTAX_ERROR error = setupError("int i[1] = {1{\n");
  assert(error == INVALID_ARRAY_INIT_CURLY_CLOSE);
}

void declDefProcNoId() {
  enum SYNTAX_ERROR error = setupError("prot 1\n");
  assert(error == NO_PROTO_ID);
}

void declDefProcProtNoOpenParen() {
  enum SYNTAX_ERROR error = setupError("prot a -\n");
  assert(error == INVALID_PROTO_PAREN_OPEN);
}

void declDefProcProtoInvalidParamType() {
  enum SYNTAX_ERROR error = setupError("prot a(&null)\n");
  assert(error == INVALID_PROTO_PARAM_TYPE);
}

// this error does not exist!
// changed the error check to correct this.
// good for documentation for the grammar, i guess.
void declDefProcProtoNoParamId() {
  enum SYNTAX_ERROR error = setupError("prot b(int 1)\n");
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}

void declDefProcProtoNoValidTokenAfterType() {
  enum SYNTAX_ERROR error = setupError("prot c(int{\n");
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}

void declDefProcProtoUnclosedArrayParam() {
  enum SYNTAX_ERROR error = setupError("prot d(int[[\n");
  assert(error == INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE);
}

void declDefProcProtoInvalid2dArrayOpen() {
  enum SYNTAX_ERROR error = setupError("prot e(int[]])\n");
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE);
}

void declDefProcProtoInvalid2dArrayClose() {
  enum SYNTAX_ERROR error = setupError("prot f(int [][[)\n");
  assert(error == INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE);
}

void declDefProcProtoInvalid3dArray() {
  enum SYNTAX_ERROR error = setupError("prot g(int [][][)\n");
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declDefProcProtoMultiParams() {
  enum SYNTAX_ERROR error = setupError("prot h(int [], int[], int[][][])\n");
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declDefProcProtoNoParenClose() {
  enum SYNTAX_ERROR error = setupError("prot i(int(\n");
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}

void declDefProcProtoTwoProts() {
  enum SYNTAX_ERROR error = setupError("prot j(int) prot k(2(\n");
  assert(error == INVALID_PROTO_PARAM_TYPE);
}

void declDefProcDefDefWorksToo() {
  enum SYNTAX_ERROR error = setupError("def init)\n");
  assert(error == INVALID_DEF_PAREN_OPEN);

  enum SYNTAX_ERROR error2 = setupError("def 8\n");
  assert(error2 == NO_DEF_ID);
}

void declDefProcDefNoParenOpen() {
  enum SYNTAX_ERROR error = setupError("def init[\n");
  assert(error == INVALID_DEF_PAREN_OPEN);
}

void declDefProcDefBadParamType() {
  enum SYNTAX_ERROR error = setupError("def indio(&string\n");
  assert(error == INVALID_DEF_PARAM_TYPE);
}


