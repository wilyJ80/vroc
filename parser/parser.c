#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/transition.h"
#include <stdio.h>

/**
 * prog accepts repetitions of declarations of variables (decl_list_var), or
 * procedures (decl_list_proc).
 */
void prog(FILE *fd, int *lineCount) {
  struct Token token = lexerGetNextChar(fd, lineCount);

  // Both declaration of variables and procedures start with reserved words.
  if (token.category == RSV) {
    // Valid variable declaration start tokens.
    while (token.signCode == CONST || token.signCode == CHAR ||
           token.signCode == INT || token.signCode == REAL ||
           token.signCode == BOOL) {
      declListVar(fd, lineCount, token);
    }
    // Valid procedure declaration/definition tokens
    while (token.signCode == DEF || token.signCode == PROT) {
      declDefProc(fd, lineCount, token);
    }
  }
}

/**
 * decl_list_var accepts optionally a `const`, followed by variable type, and
 * declaration of one or more variables.
 */
void declListVar(FILE *fd, int *lineCount, struct Token token) {
  // if const, read next
}
