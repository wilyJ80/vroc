#include "./lexer/lexer.h"
#include "types/types.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Error. Usage: calcula <code>\n");
    return EXIT_FAILURE;
  }

  FILE *fd;

  fd = fopen(argv[1], "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  // lexing!
  while (true) {
    struct Token token = lexerGetNextChar(fd);
    if (token.category == END_OF_FILE) {
      fclose(fd);
      return EXIT_SUCCESS;
    }
    printf("%d\n", token.category);
  }
}
