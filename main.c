#include "./lexer/lexer.h"
#include "./lexer/printer.h"
#include "./lexer/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Error. Usage: croc <code>\n");
    return EXIT_FAILURE;
  }

  FILE *fd;
  int lineCount = 1;

  fd = fopen(argv[1], "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  // lexing!
  while (true) {
    struct Token token = lexerGetNextChar(fd, &lineCount);
    if (token.category == END_OF_FILE) {
      fclose(fd);
      return EXIT_SUCCESS;
    }
    // handle malformed manually to
    // keep the printing callback simple.
    switch (token.category) {
    case MALFORMED_TOKEN:
      printf("ERROR: MALFORMED TOKEN %s ON LINE %d\n", token.lexeme, lineCount);
      exit(EXIT_FAILURE);
    }
    printToken(token);
    printf("\n");
  }
}
