#include "parser_unit_tests.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void opRelTest() {
  const char *mock_data = "==\n";
  FILE *mock_file = fmemopen((void *)mock_data, strlen(mock_data), "r");

  if (mock_file == NULL) {
    fprintf(stderr, "Error opening source file.\n");
    exit(EXIT_FAILURE);
  }

  int lineCount = 1;
  struct Token tokens[7];
  int index = 0;

  while (true) {
    struct Token token = lexerGetNextChar(mock_file, &lineCount);
    if (token.category == END_OF_FILE) {
      break;
    }
    tokens[index] = token;
    index++;
  }

  enum SYNTAX_ERROR error = op_rel(mock_file, &lineCount);
  assert(error == NO_ERROR);
}
