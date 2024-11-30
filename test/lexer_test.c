#include "lexer_test.h"
#include "../lexer/lexer.h"
#include "../lexer/types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lexerTest() {
  const char *mock_data = "def init 2 endp\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int lineCount = 1;
  struct Token tokens[5];
  int index = 0;

  while (true) {
    struct Token token = lexerGetNextChar(mock_file, &lineCount);
    if (token.category == END_OF_FILE) {
      break;
    }
    tokens[index] = token;
    index++;
  }

  assert(tokens[0].category == RSV);
  assert(strcmp(tokens[0].lexeme, "def") == 0);

  assert(tokens[1].category == RSV);
  assert(strcmp(tokens[1].lexeme, "init") == 0);

  assert(tokens[2].category == INTCON);
  assert(tokens[2].intValue == 2);

  assert(tokens[3].category == RSV);
  assert(strcmp(tokens[3].lexeme, "endp") == 0);
}

void lexerCharconTest() {
  const char *mock_data = "'\\n'\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int lineCount = 1;
  struct Token tokens[3];
  int index = 0;

  while (true) {
    struct Token token = lexerGetNextChar(mock_file, &lineCount);
    if (token.category == END_OF_FILE) {
      break;
    }
    tokens[index] = token;
    index++;
  }

  assert(tokens[0].category == CHARCON);
  assert(strcmp(tokens[0].lexeme, "'\\n'") == 0);
}

void lexerCharconTest2() {
  FILE *mock_file = fopen("./test/newline_test.proc", "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int lineCount = 1;
  struct Token tokens[2];
  int index = 0;

  while (true) {
    struct Token token = lexerGetNextChar(mock_file, &lineCount);
    if (token.category == END_OF_FILE) {
      break;
    }
    tokens[index] = token;
    index++;
  }

  assert(tokens[1].category == CHARCON);
  assert(strcmp(tokens[1].lexeme, "'\\n'") == 0);
}

void lexerCharconTest3() {
  FILE *mock_file = fopen("./test/./terminator_char_test.proc", "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int lineCount = 1;
  struct Token tokens[2];
  int index = 0;

  while (true) {
    struct Token token = lexerGetNextChar(mock_file, &lineCount);
    if (token.category == END_OF_FILE) {
      break;
    }
    tokens[index] = token;
    index++;
  }

  assert(tokens[1].category == CHARCON);
  assert(strcmp(tokens[1].lexeme, "'\\0'") == 0);
}
