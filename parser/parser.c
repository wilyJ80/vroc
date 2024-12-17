#include "parser.h"
#include "../lexer/lexer.h"
#include "syntax_error.h"
#include "token_match.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 13
#define MAX_TRANSITIONS 3
#define MAX_NONCONSUMING 3

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES + 1][MAX_TRANSITIONS] = {
    // 0: initial
    {
        {STATE_VALID_START, tkIsConstOrType, NONACCEPTING,
         INVALID_PROG_START_KEYWORD, NON_CONSUMING},
    },
    // 1: valid start
    {
        {STATE_CONST, tkIsConst, NONACCEPTING, NO_ERROR, CONSUMING},
        {STATE_DLV, tkIsType, NONACCEPTING, NO_ERROR, CONSUMING},
    },
    // 2: const
    {
        {STATE_DLV, tkIsType, NONACCEPTING, INVALID_TYPE, CONSUMING},
    },
    // 3: decl list var
    {
        {STATE_DV, tkIsId, ACCEPTING, NO_VAR_ID, CONSUMING},
    },
    // 4: decl var: accepting
    {
        {STATE_ASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING},
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING},
    },
    // 5: assign single
    {
        {STATE_ASSEND, tkIsIntconRealconOrCharcon, ACCEPTING,
         INVALID_VAR_TYPE_INIT, CONSUMING},
    },
    // 6: assigned variable, can have a list too
    {
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING},
    },
    // 7: start array declaration
    {
        {STATE_SUBS, tkIsIntconOrId, NONACCEPTING, INVALID_ARRAY_SUBSCRIPT_DEC,
         CONSUMING},
    },
    // 8: please close the bracket
    {
        {STATE_ARRCLOSE, tkIsBracketClose, ACCEPTING,
         INVALID_ARRAY_BRACKET_DEC_CLOSE, CONSUMING},
    },
    // 9: multidimensional arrays
    {
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING},
        {STATE_ARRASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING},
    },
    // 10: we start assigning something to the array
    {
        {STATE_ARRCURLYO, tkIsCurlyOpen, NONACCEPTING,
         INVALID_ARRAY_INIT_CURLY_OPEN, CONSUMING},
    },
    // 11: please give me the valid type!
    {
        {STATE_ARRINITTYPE, tkIsIntconRealconOrCharcon, NONACCEPTING,
         INVALID_ARRAY_TYPE_INIT, CONSUMING},
    },
    // 12: we can end the array or continue...
    {
        {STATE_ARRCURLYO, tkIsComma, NONACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING},
        {STATE_ARRCURLCLOSE, tkIsCurlyClose, ACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING},
    },
    // 13: terminal - finished array initialization
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
        if (possibility->isConsuming) {
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
        // parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
        currentIsAccepting = false;
        continue;
      } else {
        return possibleTransitions[currentState][0].error;
      }
    }
  }

    if (parser->token.category == END_OF_FILE && !currentIsAccepting) {
        return possibleTransitions[currentState][0].error;
    }

  return NO_ERROR;
}
