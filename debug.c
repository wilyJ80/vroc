#include "./lexer/lexer.h"
#include "types/types.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  FILE *fd;

  fd = fopen("./doc/examples/code.proc", "r");
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
    switch (token.category) {
    case ID:
      printf("<ID, %s>", token.lexeme);
      break;
    case SIGN:
      switch (token.signCode) {
      case PLUS:
        printf("<SN, PLUS>");
        break;
      case MINUS:
        printf("<SN, MINUS>");
        break;
      case STAR:
        printf("<SN, STAR>");
        break;
      case SLASH:
        printf("<SN, SLASH>");
        break;
      case ASSIGN:
        printf("<SN, ASSIGN>");
        break;
      case OPEN_PAR:
        printf("<SN, OPEN_PAR>");
        break;
      case CLOSE_PAR:
        printf("<SN, CLOSE_PAR>");
        break;
      }
      break;
    case INTCON:
      printf("<INTCON, %d>", token.intValue);
      break;
    }
  }
}
