#include "./lexer/lexer.h"
#include "./lexer/printer.h"
#include "./lexer/types.h"
#include "./parser/parser.h"
#include "./parser/syntax_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI escape codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

int main(int argc, char *argv[]) {
  /*if (argc != 2) {*/
  /*  fprintf(stderr, "Error. Usage: croc <code>\n");*/
  /*  return EXIT_FAILURE;*/
  /*}*/

  FILE *fd;
  int lineCount = 1;

  fd = fopen("./doc/examples/fatrec.proc", "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  // lexing!
  while (true) {
    struct Token token = lexerGetNextChar(fd, &lineCount);
    if (token.category == END_OF_FILE) {
      fclose(fd);
      break;
    }
    // handle malformed manually to
    // keep the printing callback simple.
    if (token.category == MALFORMED_TOKEN) {
      fprintf(stderr, RED "ERROR: MALFORMED TOKEN %s ON LINE %d\n" RESET, token.lexeme,
              lineCount);
      exit(EXIT_FAILURE);
    }
    printToken(token);
    printf("\n");
  }

  printf(GREEN "Lexing successful!" RESET "\n");

  int *lc;
  int line = 1;
  lc = &line;

  fd = fopen("./doc/examples/fatrec.proc", "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  // for integration tests, prog itself needs a previously initialized parser
  // with a token too
  struct Token token = lexerGetNextChar(fd, lc);
  struct Parser parser = {
      .fd = fd, .lineCount = lc, .token = token};

  enum SYNTAX_ERROR error = prog(&parser);
  if (error != NO_ERROR) {
    printSyntaxError(error, lc);
    exit(EXIT_FAILURE);
  }

  printf(GREEN "Parsing successful!" RESET "\n");
}
