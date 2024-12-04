#include "parser_integration_tests.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void progStartKeyword() {
  const char *mock_data = "==\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  // for integration tests, prog itself needs a previously initialized parser with a token too
  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  // example debugging:
  // printSyntaxError(error);
  assert(error == INVALID_PROG_START_KEYWORD);
}

void declListVarInvalidType() {
  const char *mock_data = "const string\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_TYPE);
}

void declVarNoId() {
  const char *mock_data = "const int 5\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_VAR_ID);
}
