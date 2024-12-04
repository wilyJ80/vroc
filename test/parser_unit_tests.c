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

  // For unit tests of individual functions like this one, that don't involve consuming tokens before, let's manually call the lexer (in the final program, tokens will be consumed before the function)
  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file,
    .lineCount = lineCount,
    .token = token
  };

  enum SYNTAX_ERROR error = op_rel(parser);
  // example debugging:
  // printSyntaxError(error);
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

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file,
    .lineCount = lineCount,
    .token = token
  };

  enum SYNTAX_ERROR error = op_rel(parser);
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

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file,
    .lineCount = lineCount,
    .token = token
  };

  enum SYNTAX_ERROR intcon = fator(parser);
  assert(intcon == NO_ERROR);

  token = lexerGetNextChar(mock_file, lineCount);
  parser.token = token;
  enum SYNTAX_ERROR realcon = fator(parser);
  assert(intcon == NO_ERROR);

  token = lexerGetNextChar(mock_file, lineCount);
  parser.token = token;
  enum SYNTAX_ERROR charcon = fator(parser);
  assert(charcon == NO_ERROR);

  token = lexerGetNextChar(mock_file, lineCount);
  parser.token = token;
  enum SYNTAX_ERROR error = fator(parser);
  assert(error == NO_FACTOR_VALID_START_SYMBOL);
}

/*void fatorNegFatorTest() {*/
/*  const char *mock_data = "2 !2.2 !)\n";*/
/*  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");*/
/**/
/*  if (mock_file == NULL) {*/
/*    fprintf(stderr, "Error opening source file.\n");*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  int *lineCount;*/
/*  int line = 1;*/
/*  lineCount = &line;*/
/**/
/*  enum SYNTAX_ERROR intcon = fator(mock_file, lineCount);*/
/*  assert(intcon == NO_ERROR);*/
/*  // recursive fator call*/
/*  enum SYNTAX_ERROR realcon = fator(mock_file, lineCount);*/
/*  assert(realcon == NO_ERROR);*/
/**/
/*  enum SYNTAX_ERROR error = fator(mock_file, lineCount);*/
/*  assert(error == NO_FACTOR_AFTER_BANG);*/
/*}*/
/**/
/*void fatorArrayOutroTest() {*/
/*  const char *mock_data = "id]\n";*/
/*  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");*/
/**/
/*  if (mock_file == NULL) {*/
/*    fprintf(stderr, "Error opening source file.\n");*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  int *lineCount;*/
/*  int line = 1;*/
/*  lineCount = &line;*/
/**/
/*  enum SYNTAX_ERROR id = fator(mock_file, lineCount);*/
/*  assert(id == NO_ERROR);*/
/*}*/
/**/
/*void fatorArrayOutroTest2() {*/
/*  const char *mock_data = "id[1[\n";*/
/*  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");*/
/**/
/*  if (mock_file == NULL) {*/
/*    fprintf(stderr, "Error opening source file.\n");*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  int *lineCount;*/
/*  int line = 1;*/
/*  lineCount = &line;*/
/**/
/*  enum SYNTAX_ERROR id = fator(mock_file, lineCount);*/
/*  assert(id == INVALID_FACTOR_ARRAY_BRACKET_CLOSE);*/
/*}*/
/**/
/*void fatorArrayUniTest() {*/
/*  const char *mock_data = "id[1] id] id[1[\n";*/
/*  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");*/
/**/
/*  if (mock_file == NULL) {*/
/*    fprintf(stderr, "Error opening source file.\n");*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  int *lineCount;*/
/*  int line = 1;*/
/*  lineCount = &line;*/
/**/
/*  enum SYNTAX_ERROR id = fator(mock_file, lineCount);*/
/*  assert(id == NO_ERROR);*/
/**/
/*  enum SYNTAX_ERROR error = fator(mock_file, lineCount);*/
/*  assert(error == NO_ERROR);*/
/**/
/*  enum SYNTAX_ERROR error2 = fator(mock_file, lineCount);*/
/*  assert(error2 == INVALID_FACTOR_ARRAY_BRACKET_CLOSE);*/
/*}*/
/**/
/*void fatorArrayMultTest() {*/
/*  // TODO: ?*/
/*}*/
/**/
/*void fatorSingle() {*/
/*  const char *mock_data = "fator funciona bem\n";*/
/*  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");*/
/**/
/*  if (mock_file == NULL) {*/
/*    fprintf(stderr, "Error opening source file.\n");*/
/*    exit(EXIT_FAILURE);*/
/*  }*/
/**/
/*  int *lineCount;*/
/*  int line = 1;*/
/*  lineCount = &line;*/
/**/
/*  enum SYNTAX_ERROR error = fator(mock_file, lineCount);*/
/*  assert(error == NO_ERROR);*/
/**/
/*  // will consume next fator*/
/*  enum SYNTAX_ERROR error1 = fator(mock_file, lineCount);*/
/*  assert(error1 == NO_ERROR);*/
/*}*/
