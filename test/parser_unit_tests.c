#include "parser_unit_tests.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void opRelTest() {
  const char *mock_data = "==\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  enum SYNTAX_ERROR error = op_rel(mock_file, lineCount);
  assert(error == NO_ERROR);
}

void opRelTest2() {
  const char *mock_data = "=!\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  enum SYNTAX_ERROR error = op_rel(mock_file, lineCount);
  assert(error == INVALID_OPERATOR);
}
