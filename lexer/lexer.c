#include "lexer.h"
#include "../util/char.h"
#include "transition.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEXEME_SIZE 50
#define MAX_STATES 44
#define MAX_TRANSITIONS 20

void error(char msg[]) {
  printf("%s\n", msg);
  exit(EXIT_FAILURE);
}

struct Token lexerGetNextChar(FILE *fd, int *lineCount) {
  struct Transition transitionTable[MAX_STATES + 1][MAX_TRANSITIONS + 1] = {
      // Target state, verification callback, category, isOther, sign type if
      // category is sign
      // State 0
      {{0, isNonPrinting, COMMENT_IGNORE, NOT_OTHER, NOT_SYMBOL},
       {1, isUnderscore, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {2, isAlpha, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {4, isDigit, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {9, isSingleQuote, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {14, isDoubleQuote, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {17, isSlash, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {19, isPlus, SIGN, NOT_OTHER, PLUS},
       {20, isMinus, SIGN, NOT_OTHER, MINUS},
       {21, isStar, SIGN, NOT_OTHER, STAR},
       {23, isParenOpen, SIGN, NOT_OTHER, OPEN_PAR},
       {24, isParenClose, SIGN, NOT_OTHER, CLOSE_PAR},
       {25, isBracketOpen, SIGN, NOT_OTHER, OPEN_BRACK},
       {26, isBracketClose, SIGN, NOT_OTHER, CLOSE_BRACK},
       {27, isComma, SIGN, NOT_OTHER, COMMA},
       {28, isEqual, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {31, isRef, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {34, isPipe, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {36, isSmallerThan, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {39, isLargerThan, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {42, isNeg, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 1
      {{1, isUnderscore, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {2, isAlpha, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 2
      {{2, isAlnumOrUnderscore, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {3, isNotAlnumOrUnderscore, ID, IS_OTHER, NOT_SYMBOL}},
      // State 3: accepting
      {},
      // State 4
      {{4, isDigit, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {5, isNotDigitNorPeriod, INTCON, IS_OTHER, NOT_SYMBOL},
       {6, isPeriod, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 5: accepting
      {},
      // State 6
      {{7, isDigit, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 7
      {{7, isDigit, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {8, isNotDigit, REALCON, IS_OTHER, NOT_SYMBOL}},
      // State 8: accepting
      {},
      // State 9
      {{10, isIsPrint, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {12, isNewline, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {13, isTerminating, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 10
      {{11, isSingleQuote, CHARCON, NOT_OTHER, NOT_SYMBOL}},
      // State 11: accepting
      {},
      // State 12
      {{11, isSingleQuote, CHARCON, NOT_OTHER, NOT_SYMBOL}},
      // State 13
      {{11, isSingleQuote, CHARCON, NOT_OTHER, NOT_SYMBOL}},
      // State 14
      {{15, isDoubleQuote, STRINGCON, NOT_OTHER, NOT_SYMBOL},
       {16, isIsPrint, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL}},
      // State 15: accepting
      {},
      // State 16
      {{16, isIsPrint, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {17, isDoubleQuote, STRINGCON, NOT_OTHER, NOT_SYMBOL}},
      // State 17
      {{18, isSlash, NON_ACCEPTING, NOT_OTHER, NOT_SYMBOL},
       {22, isNotSlash, SIGN, IS_OTHER, SLASH}},
      // State 18
      {{0, isNewline, COMMENT_IGNORE, NOT_OTHER, NOT_SYMBOL},
       {18, isNotNewline, NON_ACCEPTING, IS_OTHER, NOT_SYMBOL}},
      // State 19: accepting
      {},
      // State 20: accepting
      {},
      // State 21: accepting
      {},
      // State 22: accepting
      {},
      // State 23: accepting
      {},
      // State 24: accepting
      {},
      // State 25: accepting
      {},
      // State 26: accepting
      {},
      // State 27: accepting
      {},
      // State 28
      {{29, isEqual, SIGN, NOT_OTHER, COMPARISON},
       {30, isNotEqual, SIGN, IS_OTHER, ASSIGN}},
      // State 29: accepting
      {},
      // State 30: accepting
      {},
      // State 31
      {{32, isRef, SIGN, NOT_OTHER, REF}, {33, isNotRef, SIGN, IS_OTHER, AND}},
      // State 32: accepting
      {},
      // State 33: accepting
      {},
      // State 34:
      {{35, isPipe, SIGN, NOT_OTHER, OR}},
      // State 35: accepting
      {},
      // State 36
      {{37, isEqual, SIGN, NOT_OTHER, SMALLER_EQUAL},
       {38, isNotEqual, SIGN, IS_OTHER, SMALLER_THAN}},
      // State 37: accepting
      {},
      // State 38: accepting
      {},
      // State 39
      {{40, isEqual, SIGN, NOT_OTHER, LARGER_EQUAL},
       {41, isNotEqual, SIGN, IS_OTHER, LARGER_THAN}},
      // State 40: accepting
      {},
      // State 41: accepting
      {},
      // State 42
      {{43, isEqual, SIGN, NOT_OTHER, DIFFERENT},
       {44, isNotEqual, SIGN, IS_OTHER, NEGATION}},
      // State 43: accepting
      {},
      // State 44: accepting
      {}};

  int state = 0;
  char lexeme[MAX_LEXEME_SIZE] = "";
  int lexemeSize = 0;
  struct Token token;
  token.category = NON_ACCEPTING;

  while (true) {
    char ch = fgetc(fd);
    if (ch == EOF) {
      token.category = END_OF_FILE;
      return token;
    }

    if (ch == '\n') {
      (*lineCount)++;
    }

    bool foundTransition = false;
    for (int possibleTransition = 0;
         possibleTransition < MAX_TRANSITIONS + 1 &&
         transitionTable[state][possibleTransition].charMatch != NULL;
         possibleTransition++) {

      if (transitionTable[state][possibleTransition].charMatch(ch)) {
        foundTransition = true;
        token.category = transitionTable[state][possibleTransition].category;

        bool tokenWasBuilt = handleTransitionAndWasTokenBuilt(
            fd, ch, &token, lexeme, &lexemeSize,
            transitionTable[state][possibleTransition], lineCount, state);

        if (tokenWasBuilt) {
          return token;
        }
        // state advancing
        state = transitionTable[state][possibleTransition].nextState;
      }
    }

    if (!foundTransition) {
      lexeme[lexemeSize] = ch;
      lexeme[++lexemeSize] = '\0';
      token.category = MALFORMED_TOKEN;
      strcpy(token.lexeme, lexeme);
      return token;
    }
  }
}
