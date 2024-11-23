#include "./lexer/lexer.h"
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

  fd = fopen(argv[1], "r");
  if (fd == NULL) {
    fprintf(stderr, "Error opening file\n");
    return EXIT_FAILURE;
  }

  int lineCount = 1;

  // lexing!
  while (true) {
    struct Token token = lexerGetNextChar(fd, &lineCount);
    if (token.category == END_OF_FILE) {
      fclose(fd);
      return EXIT_SUCCESS;
    }
    switch (token.category) {
    case MALFORMED_TOKEN:
      printf("ERROR: MALFORMED TOKEN %s ON LINE %d\n", token.lexeme, lineCount);
      exit(EXIT_FAILURE);
    case ID:
      if (strcmp(token.lexeme, "const") == 0) {
        printf("<RSV, CONST>");
        break;
      } else if (strcmp(token.lexeme, "pr") == 0) {
        printf("<RSV, PR>");
        break;
      } else if (strcmp(token.lexeme, "init") == 0) {
        printf("<RSV, INIT>");
        break;
      } else if (strcmp(token.lexeme, "endp") == 0) {
        printf("<RSV, ENDP>");
        break;
      } else if (strcmp(token.lexeme, "char") == 0) {
        printf("<RSV, CHAR>");
        break;
      } else if (strcmp(token.lexeme, "int") == 0) {
        printf("<RSV, INT>");
        break;
      } else if (strcmp(token.lexeme, "real") == 0) {
        printf("<RSV, REAL>");
        break;
      } else if (strcmp(token.lexeme, "bool") == 0) {
        printf("<RSV, BOOL>");
        break;
      } else if (strcmp(token.lexeme, "do") == 0) {
        printf("<RSV, DO>");
        break;
      } else if (strcmp(token.lexeme, "while") == 0) {
        printf("<RSV, WHILE>");
        break;
      } else if (strcmp(token.lexeme, "endw") == 0) {
        printf("<RSV, ENDW>");
        break;
      } else if (strcmp(token.lexeme, "var") == 0) {
        printf("<RSV, VAR>");
        break;
      } else if (strcmp(token.lexeme, "from") == 0) {
        printf("<RSV, FROM>");
        break;
      } else if (strcmp(token.lexeme, "to") == 0) {
        printf("<RSV, TO>");
        break;
      } else if (strcmp(token.lexeme, "dt") == 0) {
        printf("<RSV, DT>");
        break;
      } else if (strcmp(token.lexeme, "by") == 0) {
        printf("<RSV, BY>");
        break;
      } else if (strcmp(token.lexeme, "if") == 0) {
        printf("<RSV, IF>");
        break;
      } else if (strcmp(token.lexeme, "endv") == 0) {
        printf("<RSV, ENDV>");
        break;
      } else if (strcmp(token.lexeme, "elif") == 0) {
        printf("<RSV, ELIF>");
        break;
      } else if (strcmp(token.lexeme, "else") == 0) {
        printf("<RSV, ELSE>");
        break;
      } else if (strcmp(token.lexeme, "endi") == 0) {
        printf("<RSV, ENDI>");
        break;
      } else if (strcmp(token.lexeme, "getout") == 0) {
        printf("<RSV, GETOUT>");
        break;
      } else if (strcmp(token.lexeme, "getint") == 0) {
        printf("<RSV, GETINT>");
        break;
      } else if (strcmp(token.lexeme, "getchar") == 0) {
        printf("<RSV, GETCHAR>");
        break;
      } else if (strcmp(token.lexeme, "getreal") == 0) {
        printf("<RSV, GETREAL>");
        break;
      } else if (strcmp(token.lexeme, "putint") == 0) {
        printf("<RSV, PUTINT>");
        break;
      } else if (strcmp(token.lexeme, "putchar") == 0) {
        printf("<RSV, PUTCHAR>");
        break;
      } else if (strcmp(token.lexeme, "putreal") == 0) {
        printf("<RSV, PUTREAL>");
        break;
      }
      printf("<ID, %s>", token.lexeme);
      break;
    case CHARCON:
      printf("<CHARCON, %c>", token.lexeme[1]);
      break;
    case STRINGCON:
      printf("<STRINGCON, %s>", token.lexeme);
      break;
    case INTCON:
      printf("<INTCON, %d>", token.intValue);
      break;
    case REALCON:
      printf("<REALCON, %lf>", token.doubleValue);
    case SIGN:
      switch (token.signCode) {
      case ASSIGN:
        printf("<SN, ASSIGN>");
        break;
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
      case OPEN_PAR:
        printf("<SN, OPEN_PAR>");
        break;
      case CLOSE_PAR:
        printf("<SN, CLOSE_PAR>");
        break;
      case OPEN_BRACK:
        printf("<SN, OPEN_BRACK>");
        break;
      case CLOSE_BRACK:
        printf("<SN, CLOSE_BRACK>");
        break;
      case COMMA:
        printf("<SN, COMMA>");
        break;
      case COMPARISON:
        printf("<SN, COMPARISON>");
        break;
      case REF:
        printf("<SN, REF>");
        break;
      case AND:
        printf("<SN, AND>");
        break;
      case OR:
        printf("<SN, OR>");
        break;
      case SMALLER_THAN:
        printf("<SN, SMALLER_THAN>");
        break;
      case LARGER_THAN:
        printf("<SN, LARGER_THAN>");
        break;
      case NEGATION:
        printf("<SN, NEGATION>");
        break;
      case DIFFERENT:
        printf("<SN, DIFFERENT>");
        break;
      case SMALLER_EQUAL:
        printf("<SN, SMALLER_EQUAL>");
        break;
      case LARGER_EQUAL:
        printf("<SN, LARGER_EQUAL>");
        break;
      }
      break;
    }
    printf("\n");
  }
}
