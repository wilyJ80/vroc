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

  // for integration tests, prog itself needs a previously initialized parser
  // with a token too
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

void declVarArrayInvalidSubscript() {
  const char *mock_data = "const int i[2.2\n";
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
  assert(error == INVALID_ARRAY_SUBSCRIPT_DEC);
}

void declVarArrayDidntClose() {
  const char *mock_data = "const int i[2[\n";
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
  assert(error == INVALID_ARRAY_BRACKET_DEC_CLOSE);
}

void declVarBadInit() {
  const char *mock_data = "const int i = i\n";
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
  assert(error == INVALID_VAR_TYPE_INIT);
}

void declListVarMulti() {
  const char *mock_data = "const int i = 1, a = 2\n";
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
  assert(error == NO_ERROR);
}

void declListVarMultiFail() {
  const char *mock_data = "const int i = 1, a = a\n";
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
  assert(error == INVALID_VAR_TYPE_INIT);
}

void declVarArrayMultiTooMany() {
  const char *mock_data = "int i[2][3][4]\n";
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
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declVarArrayBadInitCurly() {
  const char *mock_data = "const int i[2] = [1\n";
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
  assert(error == INVALID_ARRAY_INIT_CURLY_OPEN);
}

void declVarArrayInvalidTypeInit() {
  const char *mock_data = "const int i[2] = {id\n";
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
  assert(error == INVALID_ARRAY_TYPE_INIT);
}

void declVarArrayMultiInit() {
  const char *mock_data = "int i[4] = {3, 4, 4, 4}\n";
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
  assert(error == NO_ERROR);
}

void declVarArrayBadClose() {
  const char *mock_data = "int i[1] = {1{\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_ARRAY_INIT_CURLY_CLOSE);
}

void declDefProcNoId() {
  const char *mock_data = "prot 1\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_PROTO_ID);
}

void declDefProcProtNoOpenParen() {
  const char *mock_data = "prot a -\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_PROTO_PAREN_OPEN);
}

void declDefProcProtoInvalidParamType() {
  const char *mock_data = "prot a(&null)\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_PROTO_PARAM_TYPE);
}

// this error does not exist!
// changed the error check to correct this.
// good for documentation for the grammar, i guess.
void declDefProcProtoNoParamId() {
  const char *mock_data = "prot b(int 1)\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}

void declDefProcProtoNoValidTokenAfterType() {
  const char *mock_data = "prot c(int{\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}

void declDefProcProtoUnclosedArrayParam() {
  const char *mock_data = "prot d(int[[\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE);
}

void declDefProcProtoInvalid2dArrayOpen() {
  const char *mock_data = "prot e(int[]])\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE);
}

void declDefProcProtoInvalid2dArrayClose() {
  const char *mock_data = "prot f(int [][[)\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
      .fd = mock_file, .lineCount = lineCount, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE);
}

void declDefProcProtoInvalid3dArray() {
  const char *mock_data = "prot g(int [][][)\n";
  FILE *mock_file = fmemopen((void*)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file, .lineCount = lineCount, .token = token
  };

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declDefProcProtoMultiParams() {
  const char *mock_data = "prot h(int [], int[], int[][][])\n";

  FILE *mock_file = fmemopen((void*)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file, .lineCount = lineCount, .token = token
  };

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == INVALID_ARRAY_DIMENSION_DECLARATION);
}

void declDefProcProtoNoParenClose() {
  const char *mock_data = "prot i(int(\n";

  FILE *mock_file = fmemopen((void*)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file\n");
    exit(EXIT_FAILURE);
  }

  int *lineCount;
  int line = 1;
  lineCount = &line;

  struct Token token = lexerGetNextChar(mock_file, lineCount);
  struct Parser parser = {
    .fd = mock_file, .lineCount = lineCount, .token = token
  };

  enum SYNTAX_ERROR error = prog(&parser);
  assert(error == NO_PROTO_VALID_TOKEN_AFTER_TYPE);
}
