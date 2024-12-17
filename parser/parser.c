#include "parser.h"
#include "../lexer/lexer.h"
#include "syntax_error.h"
#include "token_match.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 13
#define MAX_TRANSITIONS 3
#define MAX_NONCONSUMING 3

enum STATE_ALIAS nonConsumingStates[MAX_NONCONSUMING] = {
    STATE_INITIAL, STATE_VALID, STATE_CONST, STATE_DLV};

bool isNonconsuming(enum STATE_ALIAS alias) {
  bool foundState = false;
  for (int i = 0; i < MAX_NONCONSUMING; i++) {
    if (alias == nonConsumingStates[i]) {
      foundState = !foundState;
    }
  }
  return foundState;
}

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES + 1][MAX_TRANSITIONS] = {
    // 0: initial
    {
        {STATE_VALID_START, tkIsConstOrType, NONACCEPTING,
         INVALID_PROG_START_KEYWORD},
    },
    // 1: valid start
    {
        {STATE_CONST, tkIsConst, NONACCEPTING, NO_ERROR},
        {STATE_DLV, tkIsType, NONACCEPTING, NO_ERROR},
    },
    // 1: const
    {
        {STATE_DLV, tkIsType, NONACCEPTING, INVALID_TYPE},
    },
    // 2: decl list var
    {
        {STATE_DV, tkIsId, ACCEPTING, NO_VAR_ID},
    },
    // 3: decl var: accepting
    {
        {STATE_ASS, tkIsAssign, NONACCEPTING, NO_ERROR},
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR},
    },
    // 4: assign single
    {
        {STATE_ASSEND, tkIsIntconRealconOrCharcon, ACCEPTING,
         INVALID_VAR_TYPE_INIT},
    },
    // 5: assigned variable, can have a list too
    {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR},
    // 6: start array declaration
    {
        {STATE_SUBS, tkIsIntconOrId, NONACCEPTING, INVALID_ARRAY_SUBSCRIPT_DEC},
    },
    // 7: please close the bracket
    {
        {STATE_ARRCLOSE, tkIsBracketClose, ACCEPTING,
         INVALID_ARRAY_BRACKET_DEC_CLOSE},
    },
    // 8: multidimensional arrays
    {
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR},
        {STATE_ARRASS, tkIsAssign, NONACCEPTING, NO_ERROR},
    },
    // 9: we start assigning something to the array
    {
        {STATE_ARRCURLYO, tkIsCurlyOpen, NONACCEPTING,
         INVALID_ARRAY_INIT_CURLY_OPEN},
    },
    // 10: please give me the valid type!
    {
        {STATE_ARRINITTYPE, tkIsIntconRealconOrCharcon, NONACCEPTING,
         INVALID_ARRAY_TYPE_INIT},
    },
    // 11: we can end the array or continue...
    {
        {STATE_ARRCURLYO, tkIsComma, NONACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM},
        {STATE_ARRCURLCLOSE, tkIsCurlyClose, ACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM},
    },
    // 12: terminal - finished array initialization
    {},
};

enum SYNTAX_ERROR parse(struct Parser *parser) {

  // the parser starts with an initial consumed token, file and line count
  enum STATE_ALIAS currentState = STATE_INITIAL;
  bool currentIsAccepting = false;

  while (parser->token.category != END_OF_FILE) {
    bool transitionFound = false;

    for (int possibleTransition = 0;
         possibleTransition < MAX_TRANSITIONS + 1 &&
         possibleTransitions[currentState][possibleTransition].matchFn != NULL;
         possibleTransition++) {

      struct ParserTransition *possibility =
          &possibleTransitions[currentState][possibleTransition];

      if (possibility->matchFn(parser->token)) {
        if (!isNonconsuming(currentState)) {
          parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
        }
        currentState = possibility->targetState;
        currentIsAccepting = possibility->isAccepting;
        transitionFound = true;
        break;
      }
    }

    if (!transitionFound) {
      if (currentIsAccepting) {
        currentState = STATE_INITIAL;
        parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
        continue;
      } else {
        return possibleTransitions[currentState][0].error;
      }
    }
  }

  return NO_ERROR;
}
