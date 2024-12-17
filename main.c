#include "./lexer/lexer.h"
#include "./lexer/printer.h"
#include "./lexer/types.h"
#include "./parser/parser.h"
#include "./parser/syntax_error.h"
#include "./parser/symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI escape codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

#define HYPHENROW "------"

int main(int argc, char *argv[]) {
  /*if (argc != 2) {*/
  /*  fprintf(stderr, "Error. Usage: croc <code>\n");*/
  /*  return EXIT_FAILURE;*/
  /*}*/

  FILE *fd;
  int lineCount = 1;

  //fd = fopen(argv[1], "r");
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
      fprintf(stderr, RED "ERROR: MALFORMED TOKEN %s ON LINE %d\n" RESET,
              token.lexeme, lineCount);
      exit(EXIT_FAILURE);
    }
    printToken(token);
    printf("\n");
  }

  printf(GREEN "Lexing successful!" RESET "\n");

  int *lc;
  int line = 1;
  lc = &line;

  // opening the file again...
  FILE *f2;
  //f2 = fopen(argv[1], "r");
  f2 = fopen("./doc/examples/fatrec.proc", "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  struct SymbolTable table = initTable();

  // for integration tests, prog itself needs a previously initialized parser
  // with a token too
  struct Token token = lexerGetNextChar(fd, lc);
  struct Parser parser = {.fd = fd, .lineCount = lc, .token = token, .symbolTable = table};

  enum SYNTAX_ERROR error = prog(&parser);
  if (error != NO_ERROR) {
    printSyntaxError(error, lc);
    exit(EXIT_FAILURE);
  }

  printf(GREEN "Parsing successful!" RESET "\n");
  printf(GREEN "Displaying symbol table..." RESET "\n");

  for (int i = 0; i < 13; i++) {
    printf("+" HYPHENROW);
  }
  printf("+"
         "\n");

  const char *header[13] = {"LEXEMA", "ESCOPO", " TIPO ", " CAT  ", "PASSGM",
                            "ZUMBI ", "ARRAY ", " DIM1 ", " DIM2 ", "ECONST",
                            "VCONST", "ENDRC ", "ROTULO"};

  for (int i = 0; i < 13; i++) {
    printf("|"
           "%s",
           header[i]);
  }
  printf("|"
         "\n");

  for (int i = 0; i < 13; i++) {
    printf("+" HYPHENROW);
  }
  printf("+"
         "\n");

  // display rows here
  if (parser.symbolTable.top > -1) {
    printf("%d\n", parser.symbolTable.row[0].category);
    for (int i = 0; i < parser.symbolTable.top; i++) {
      struct Row *row = &parser.symbolTable.row[i];
      printf("|%6s", row->lexeme);
      printf("|%6d", row->scope);
      printf("|%6d", row->type);
      printf("|%6d", row->category);
      printf("|%6d", row->passage);
      printf("|%6d", row->zombie);
      printf("|%6d", row->array);
      printf("|%6d", row->dim1);
      printf("|%6d", row->dim2);
      printf("|%6d", row->isConst);
      switch (row->constValue.type) {
      case TYPE_INT:
        printf("|%6d", row->constValue.intValue);
        break;
      case TYPE_REAL:
        printf("|%6lf", row->constValue.doubleValue);
        break;
      case TYPE_CHAR:
        printf("|%6c", row->constValue.charValue);
        break;
      case TYPE_NA:
        printf("|NA    ");
        break;
      default:
        break;
      }
      printf("|%6d", row->refAddress);
      printf("|%6d", row->procLabel);

      i++;
    }
    printf("|\n");
  }

  for (int i = 0; i < 13; i++) {
    printf("+" HYPHENROW);
  }
  printf("+"
         "\n");
}
