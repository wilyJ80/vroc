#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/transition.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * prog accepts repetitions of declarations of variables (decl_list_var), or
 * procedures (decl_list_proc).
 */
void prog(FILE *fd, int *lineCount) {
  struct Token token = lexerGetNextChar(fd, lineCount);

  // Both declaration of variables and procedures start with reserved words.
  if (token.category == RSV) {
    // Valid variable declaration start tokens
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
  if (token.signCode == CONST) {
    token = lexerGetNextChar(fd, lineCount);
  }
  // get remaining parts of variable declaration
  tipo(fd, lineCount, token);
  declVar(fd, lineCount, token);
  // can accept more variable declarations
  while (token.category == SIGN && token.signCode == COMMA) {
    // consume next
    token = lexerGetNextChar(fd, lineCount);
    declVar(fd, lineCount, token);
  }
}

/**
 * type must be valid
 */
void tipo(FILE *fd, int *lineCount, struct Token token) {
  if (token.signCode == CHAR || token.signCode == INT ||
      token.signCode == REAL || token.signCode == BOOL) {
    struct Token token = lexerGetNextChar(fd, lineCount);

  } else {
    // report error if type not detected
    fprintf(stderr, "Syntax error: variable type not detected\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * We already dealt with type.
 * Variable declaration is valid:
 * Simply declaring a identifier, or optionally initializing it.
 * Can declare as array using curly braces with size given by number or id. Can
 * optionally initialize it with square brackets.
 */
void declVar(FILE *fd, int *lineCount, struct Token token) {
  // id mandatory.
  if (token.category != ID) {
    fprintf(stderr, "Syntax error: expected variable ID\n");
    exit(EXIT_FAILURE);
  }
  // consume next
  token = lexerGetNextChar(fd, lineCount);

  // handle simple variable or array
  // simple variable
  if (token.category == SIGN && token.signCode == ASSIGN) {
    // assignment detected, check next token
    token = lexerGetNextChar(fd, lineCount);
    if (token.category == CHARCON || token.category == STRINGCON ||
        token.category == INTCON || token.category == REALCON) {
      // consume next
      token = lexerGetNextChar(fd, lineCount);
    } else {
      fprintf(stderr, "Syntax error: expected valid variable initialization\n");
      exit(EXIT_FAILURE);
    }

    // array: should deal with multidimensional arrays (declaration) and
    // initialization
  } else if (token.category == SIGN && token.signCode == OPEN_BRACK) {
    arrayDeclaration(fd, lineCount, token);
  }
}

void arrayDeclaration(FILE *fd, int *lineCount, struct Token token) {
  while (token.category == SIGN && token.signCode == OPEN_BRACK) {
    // consume next
    token = lexerGetNextChar(fd, lineCount);
    // valid array initializer
    if (token.category != ID && token.category != INTCON) {
      fprintf(stderr, "Syntax error: expected valid array size\n");
      exit(EXIT_FAILURE);
    } else {
      // consume next
      token = lexerGetNextChar(fd, lineCount);
      if (token.category != SIGN && token.signCode != CLOSE_BRACK) {

        fprintf(stderr, "Syntax error: expected array bracket closing\n");
        exit(EXIT_FAILURE);
      } else {
        token = lexerGetNextChar(fd, lineCount);

        if (token.category == SIGN && token.signCode == OPEN_BRACK) {
          // dealing with multidimensional arrays
          continue;

        } else if (token.category == SIGN && token.signCode == ASSIGN) {
          // consume next
          token = lexerGetNextChar(fd, lineCount);
          if (token.category != SIGN && token.signCode != OPEN_CURLY) {
          
          }
          // it's optional in the grammar.
          arrayInitialization(fd, lineCount, token);
          break;

        } else {
          // no array declaration. what to do?
        }
      }
    }
  }
}

void arrayInitialization(FILE *fd, int *lineCount, struct Token token) {}
