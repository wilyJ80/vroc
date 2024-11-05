#include "lexer.h"
#include "../util/char.h"
#include "transition.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEXEME_SIZE 50
#define MAX_STATES 47
#define MAX_TRANSITIONS 21

void error(char msg[]) {
  printf("%s\n", msg);
  exit(EXIT_FAILURE);
}

struct Token lexerGetNextChar(FILE *fd) {
  struct Transition transitionTable[MAX_STATES + 1][MAX_TRANSITIONS + 1] = {
      // Target state, verification callback, category, isOther, sign type if
      // category is sign
      // State 0
      {{0, isNonPrinting, NON_ACCEPTING, false, NONE},
       {1, isUnderscore, NON_ACCEPTING, false, NONE},
       {2, isAlpha, NON_ACCEPTING, false, NONE},
       {4, isDigit, NON_ACCEPTING, false, NONE},
       {9, isSingleQuote, NON_ACCEPTING, false, NONE},
       {16, isDoubleQuote, NON_ACCEPTING, false, NONE},
       {20, isSlash, NON_ACCEPTING, false, NONE},
       {22, isPlus, SIGN, false, PLUS},
       {23, isMinus, SIGN, false, MINUS},
       {24, isStar, SIGN, false, STAR},
       {26, isParenOpen, SIGN, false, OPEN_PAR},
       {27, isParenClose, SIGN, false, CLOSE_PAR},
       {28, isBracketOpen, SIGN, false, OPEN_BRACK},
       {29, isBracketClose, SIGN, false, CLOSE_BRACK},
       {30, isComma, SIGN, false, COMMA},
       {31, isEqual, NON_ACCEPTING, false, NONE},
       {34, isRef, NON_ACCEPTING, false, NONE},
       {37, isPipe, NON_ACCEPTING, false, NONE},
       {39, isSmallerThan, NON_ACCEPTING, false, NONE},
       {42, isLargerThan, NON_ACCEPTING, false, NONE},
       {45, isNeg, NON_ACCEPTING, false, NONE}},
      // State 1
      {{1, isUnderscore, NON_ACCEPTING, false, NONE},
       {2, isAlpha, NON_ACCEPTING, false, NONE}},
      // State 2
      {{3, isNotAlpha, ID, true, NONE}},
      // State 3: accepting
      {},
      // State 4
      {{4, isDigit, NON_ACCEPTING, false, NONE},
       {5, isNotDigit, INTCON, true, NONE},
       {6, isPeriod, NON_ACCEPTING, false, NONE}},
      // State 5: accepting
      {},
      // State 6
      {{7, isDigit, NON_ACCEPTING, false, NONE}},
      // State 7
      {{7, isDigit, NON_ACCEPTING, false, NONE},
       {8, isNotDigit, REALCON, true, NONE}},
      // State 8: accepting
      {},
      // State 9
      {{10, isIsPrint, NON_ACCEPTING, false, NONE},
       {11, isBackSlash, NON_ACCEPTING, false, NONE}},
      // State 10
      {
       {15, isSingleQuote, NON_ACCEPTING, false, NONE}
    },
      // State 11
      {}};

  int state = 0;
  char lexeme[MAX_LEXEME_SIZE] = "";
  int lexemeSize = 0;
  int lineCount = 1;
  struct Token token;
  token.category = NON_ACCEPTING;

  while (true) {
    char ch = fgetc(fd);
    if (ch == EOF) {
      token.category = END_OF_FILE;
      return token;
    }

    for (int possibleTransition = 0;
         possibleTransition < MAX_TRANSITIONS + 1 &&
         transitionTable[state][possibleTransition].charMatch != NULL;
         possibleTransition++) {

      if (transitionTable[state][possibleTransition].charMatch(ch)) {
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
  }
}
