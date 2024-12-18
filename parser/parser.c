#include "parser.h"
#include "../lexer/lexer.h"
#include "syntax_error.h"
#include "token_match.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 22
#define MAX_TRANSITIONS 3

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES + 1][MAX_TRANSITIONS] = {
    // 0: initial
    {
        {STATE_VALID_START, tkIsConstOrType, NONACCEPTING,
         INVALID_PROG_START_KEYWORD, NON_CONSUMING, NO_SET_CHECKPOINT},
        {STATE_FUNCTION, tkIsProtOrDef, NONACCEPTING,
         INVALID_PROG_START_KEYWORD, NON_CONSUMING, SET_CHECKPOINT},
    },
    // 1: valid start
    {
        {STATE_CONST, tkIsConst, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_DLV, tkIsType, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 2: const
    {
        {STATE_DLV, tkIsType, NONACCEPTING, INVALID_TYPE, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 3: decl list var
    {
        {STATE_DV, tkIsId, ACCEPTING, NO_VAR_ID, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 4: decl var: accepting
    {
        {STATE_ASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 5: assign single
    {
        {STATE_ASSEND, tkIsIntconRealconOrCharcon, ACCEPTING,
         INVALID_VAR_TYPE_INIT, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 6: assigned variable, can have a list too
    {
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 7: start array declaration
    {
        {STATE_SUBS, tkIsIntconOrId, NONACCEPTING, INVALID_ARRAY_SUBSCRIPT_DEC,
         CONSUMING, NO_SET_CHECKPOINT},
    },
    // 8: please close the bracket
    {
        {STATE_ARRCLOSE, tkIsBracketClose, ACCEPTING,
         INVALID_ARRAY_BRACKET_DEC_CLOSE, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 9: multidimensional arrays
    {
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_ARRASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 10: we start assigning something to the array
    {
        {STATE_ARRCURLYO, tkIsCurlyOpen, NONACCEPTING,
         INVALID_ARRAY_INIT_CURLY_OPEN, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 11: please give me the valid type!
    {
        {STATE_ARRINITTYPE, tkIsIntconRealconOrCharcon, NONACCEPTING,
         INVALID_ARRAY_TYPE_INIT, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 12: we can end the array or continue...
    {
        {STATE_ARRCURLYO, tkIsComma, NONACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING, NO_SET_CHECKPOINT},
        {STATE_ARRCURLCLOSE, tkIsCurlyClose, ACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 13: terminal - finished array initialization
    {},
    // 14: checkpoint state for functions
    {
        {STATE_PROT, tkIsProt, NONACCEPTING, INVALID_FUNCTION_KEYWORD,
         CONSUMING, NO_SET_CHECKPOINT},
    },
    // 15: check for id
    {
        {STATE_PROTID, tkIsId, NONACCEPTING, NO_PROTO_ID, CONSUMING,
         NO_SET_CHECKPOINT},
    },
    // 16: check for paren open
    {
        {STATE_PROTOPAR, tkIsParenOpen, NONACCEPTING, INVALID_PROTO_PAREN_OPEN,
         CONSUMING, NO_SET_CHECKPOINT},
    },
    // 17: have something or close the prototype
    {
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT},
        {STATE_PROTREF, tkIsRef, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT},
        {STATE_PROTTYPE, tkIsType, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 18: terminal: we finished the prot
    {},
    // 19: mandatory to complete the ref
    {
        {STATE_PROTTYPE, tkIsType, NONACCEPTING, NO_PROTO_TYPE_AFTER_REF,
         CONSUMING, NO_SET_CHECKPOINT},
    },
    // 20: can open bracket, end with paren, or comma for list
    {
        {STATE_PROTOBRACK, tkIsBracketOpen, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT},
        {STATE_PROTOPAR, tkIsComma, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT},
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT},
    },
    // 21: array bracket open
    {STATE_PROTCBRACK, tkIsBracketClose, NONACCEPTING,
     INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE, CONSUMING, NO_SET_CHECKPOINT},
    // 22: ending the array param
    {
        {STATE_PROTOBRACK, tkIsBracketOpen, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_PROTOPAR, tkIsComma, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING,
         NO_SET_CHECKPOINT},
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING,
         NO_SET_CHECKPOINT},
    },
};

enum SYNTAX_ERROR parse(struct Parser *parser) {
  enum STATE_ALIAS initialState = STATE_INITIAL;

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

        if (possibility->setsCheckpoint) {
          initialState = possibility->targetState;
          currentState = possibility->targetState;
        }
        break;
      }
    }

    if (!transitionFound) {
      if (currentIsAccepting) {
        currentState = initialState;
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
