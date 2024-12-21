#include "new_parser_tests.h"
#include "parser_integration_tests.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum SYNTAX_ERROR newSetupError(const char* mockData) {
  FILE *mockFile = fmemopen((void *)mockData, strlen(mockData), "r");

  if (mockFile == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mockFile, lineCount);
  struct Parser parser = {
    .fd = mockFile, .lineCount = lineCount, .token = token
  };

  enum SYNTAX_ERROR error = parse(&parser);
  if (error) {
    printSyntaxError(error, parser.lineCount);
  }

  return error;
}

void declarationsTest() {
  const char* mockData = "int a int a, b int a, b = 5 int a = 5, b = 5 int a = 5, b[4] int b[1], c[2] int b[1] = {'a'} int c[b] = {1, 2} int c[1][b] = {1, 2, 3} string\n";
  enum SYNTAX_ERROR error = newSetupError(mockData);
  assert(error == INVALID_PROG_START_KEYWORD);
}

void protTest() {
  const char* mockData = "prot i() prot i (int) prot i(&int) prot i(&int, int[]) int\n";
  enum SYNTAX_ERROR error = newSetupError(mockData);
  assert(error == INVALID_FUNCTION_KEYWORD);
}

void defTest() {
  const char* mockData = "def init() endp int a\n";
  enum SYNTAX_ERROR error = newSetupError(mockData);
  assert(error == INVALID_FUNCTION_KEYWORD);
}

void defTestWithRepeatedVariableList() {
  const char* mockData = "def init() int a = 5, int x endp int a\n";
  enum SYNTAX_ERROR error = newSetupError(mockData);
  assert(error == INVALID_FUNCTION_KEYWORD);
}
