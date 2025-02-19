#include "transition.h"
#include "./types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORD_QTY 31

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
                                      int *lineCount, int state) {
  // No state advancing needed, state will always advance
  if (token->category == NON_ACCEPTING) {
    // TODO: handle whitespace characters on lexer function
    lexeme[*lexemeSize] = ch;
    lexeme[++(*lexemeSize)] = '\0';
    return false;

  } else if (token->category == COMMENT_IGNORE) {
    // ignore the token and reset it
    lexeme[0] = '\0';
    *lexemeSize = 0;
    return false;

  } else {
    // accepting category
    if (transition.isOther == IS_OTHER && ch != '\n') {
      ungetc(ch, fd);
    }

    // no implicit conversion allowed!
    int signCode = transition.signCode;
    if (token->category == SIGN) {
      token->signCode = signCode;

    } else if (token->category == INTCON) {
      token->intValue = atoi(lexeme);

    } else if (token->category == REALCON) {
      token->doubleValue = atof(lexeme);

    } else if (token->category == ID || token->category == STRINGCON) {
      strcpy(token->lexeme, lexeme);
      // look up reserved
      struct ReservedWord reservedKeywords[KEYWORD_QTY] = {
          {"const", CONST},   {"init", INIT},       {"endp", ENDP},
          {"char", CHAR},     {"int", INT},         {"real", REAL},
          {"bool", BOOL},     {"do", DO},           {"while", WHILE},
          {"endw", ENDW},     {"var", VAR},         {"from", FROM},
          {"to", TO},         {"dt", DT},           {"by", BY},
          {"if", IF},         {"endv", ENDV},       {"elif", ELIF},
          {"else", ELSE},     {"endi", ENDI},       {"getout", GETOUT},
          {"getint", GETINT}, {"getchar", GETCHAR}, {"getreal", GETREAL},
          {"putint", PUTINT}, {"putchar", PUTCHAR}, {"putreal", PUTREAL},
          {"getstr", GETSTR}, {"putstr", PUTSTR},   {"def", DEF},
          {"prot", PROT}};

      for (int i = 0; i < KEYWORD_QTY; i++) {
        if (strcmp(token->lexeme, reservedKeywords[i].lexeme) == 0) {
          token->category = RSV;
          strcpy(token->lexeme, reservedKeywords[i].lexeme);
          token->signCode = reservedKeywords[i].reservedCode;
          break;
        }
      }
    } else if (token->category == CHARCON) {
      lexeme[*lexemeSize] = ch;
      lexeme[++(*lexemeSize)] = '\0';
      strcpy(token->lexeme, lexeme);
    }
    return true;
  }
}
