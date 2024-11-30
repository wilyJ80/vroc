#include "parser.h"
#include "../lexer/lexer.h"
#include <stdio.h>

void prog(FILE *fd, int *lineCount) {
  struct Token token = lexerGetNextChar(fd, lineCount);

  if (token.category == RSV) {
    while (token.lexeme) {
    
    }
  }
}
