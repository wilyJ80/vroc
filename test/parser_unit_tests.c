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

void fatorConTest() {
  const char *mock_data = "2 2.2 'a' +\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  enum SYNTAX_ERROR intcon = fator(mock_file, lineCount);
  assert(intcon == NO_ERROR);
  enum SYNTAX_ERROR realcon = fator(mock_file, lineCount);
  assert(intcon == NO_ERROR);
  enum SYNTAX_ERROR charcon = fator(mock_file, lineCount);
  assert(charcon == NO_ERROR);
  enum SYNTAX_ERROR error = fator(mock_file, lineCount);
  assert(error == NO_FACTOR_VALID_START_SYMBOL);

}

void fatorNegFatorTest() {
  const char *mock_data = "2 !2.2 'b'\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  enum SYNTAX_ERROR intcon = fator(mock_file, lineCount);
  assert(intcon == NO_ERROR);
  // recursive fator call
  enum SYNTAX_ERROR realcon = fator(mock_file, lineCount);
  assert(realcon == NO_ERROR);

  enum SYNTAX_ERROR error = fator(mock_file, lineCount);
  assert(error == NO_FACTOR_VALID_START_SYMBOL);
}

void fatorArrayUniTest() {
  const char *mock_data = "id[1] id]\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  enum SYNTAX_ERROR id = fator(mock_file, lineCount);
  assert(id == NO_ERROR);

  enum SYNTAX_ERROR error = fator(mock_file, lineCount);
  assert(error == INVALID_FACTOR_ARRAY_BRACKET_OPEN);
}
