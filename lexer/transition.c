#include "../types/types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TOKEN BUILDING
 * NON ACCEPTING STATE: Increments the token (lexeme or number digits) depending
 * on the verification of type
 */

/**
 * Returns whether or not token was built. Called when a valid transition was
 * found
 */
bool handleTransitionAndWasTokenBuilt(FILE *fd, char ch, struct Token *token,
                                      char *lexeme, int *lexemeSize,
                                      struct Transition transition,
                                      int lineCount, int state) {
  // No state advancing needed, state will always advance
  if (token->category == NON_ACCEPTING) {
    // TODO: handle whitespace characters on lexer function
    lexeme[*lexemeSize] = ch;
    lexeme[++(*lexemeSize)] = '\0';
    return false;

  } else {
    // accepting category
    if (transition.isOther == IS_OTHER) {
      ungetc(ch, fd);
    }

    // no implicit conversion allowed!
    int signCode = transition.signCode;
    if (token->category == SIGN) {
      token->signCode = signCode;

    } else if (token->category == INTCON) {
      token->intValue = atoi(lexeme);

    } else if (token->category == ID) {
      strcpy(token->lexeme, lexeme);
    }
    return true;
  }
}
