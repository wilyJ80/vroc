#include "parser.h"
#include "../lexer/lexer.h"
#include "syntax_error.h"
#include "token_match.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 51
#define MAX_TRANSITIONS 11

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES + 1][MAX_TRANSITIONS] = {
    // 0: initial
    {
        {STATE_VALID_START, tkIsConstOrType, NONACCEPTING,
         INVALID_PROG_START_KEYWORD, NON_CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_FUNCTION, tkIsProtOrDef, NONACCEPTING,
         INVALID_PROG_START_KEYWORD, NON_CONSUMING, SET_CHECKPOINT, DONT_STACK},
    },
    // 1: valid start
    {
        {STATE_CONST, tkIsConst, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DLV, tkIsType, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 2: const
    {
        {STATE_DLV, tkIsType, NONACCEPTING, INVALID_TYPE, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 3: decl list var
    {
        {STATE_DV, tkIsId, ACCEPTING, NO_VAR_ID, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 4: decl var: accepting
    {
        {STATE_ASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 5: assign single
    {
        {STATE_ASSEND, tkIsIntconRealconOrCharcon, ACCEPTING,
         INVALID_VAR_TYPE_INIT, CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 6: assigned variable, can have a list too
    {
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 7: start array declaration
    {
        {STATE_SUBS, tkIsIntconOrId, NONACCEPTING, INVALID_ARRAY_SUBSCRIPT_DEC,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 8: please close the bracket
    {
        {STATE_ARRCLOSE, tkIsBracketClose, ACCEPTING,
         INVALID_ARRAY_BRACKET_DEC_CLOSE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 9: multidimensional arrays
    {
        {STATE_ARROPEN, tkIsBracketOpen, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_ARRASS, tkIsAssign, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DLV, tkIsComma, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 10: we start assigning something to the array
    {
        {STATE_ARRCURLYO, tkIsCurlyOpen, NONACCEPTING,
         INVALID_ARRAY_INIT_CURLY_OPEN, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 11: please give me the valid type!
    {
        {STATE_ARRINITTYPE, tkIsIntconRealconOrCharcon, NONACCEPTING,
         INVALID_ARRAY_TYPE_INIT, CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 12: we can end the array or continue...
    {
        {STATE_ARRCURLYO, tkIsComma, NONACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_ARRCURLCLOSE, tkIsCurlyClose, ACCEPTING,
         INVALID_ARRAY_TOKEN_AFTER_ITEM, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 13: terminal - finished array initialization
    {},
    // 14: checkpoint state for functions
    {
        {STATE_PROT, tkIsProt, NONACCEPTING, INVALID_FUNCTION_KEYWORD,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DEF, tkIsDef, NONACCEPTING, INVALID_FUNCTION_KEYWORD, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 15: check for id
    {
        {STATE_PROTID, tkIsId, NONACCEPTING, NO_PROTO_ID, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 16: check for paren open
    {
        {STATE_PROTOPAR, tkIsParenOpen, NONACCEPTING, INVALID_PROTO_PAREN_OPEN,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 17: have something or close the prototype
    {
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTREF, tkIsRef, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTTYPE, tkIsType, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_PAREN_OPEN, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 18: terminal: we finished the prot
    {},
    // 19: mandatory to complete the ref
    {
        {STATE_PROTTYPE, tkIsType, NONACCEPTING, NO_PROTO_TYPE_AFTER_REF,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 20: can open bracket, end with paren, or comma for list
    {
        {STATE_PROTOBRACK, tkIsBracketOpen, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTOPAR, tkIsComma, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_TYPE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 21: array bracket open
    {
        {STATE_PROTCBRACK, tkIsBracketClose, NONACCEPTING,
         INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 22: ending the array param
    {
        {STATE_PROTOBRACK, tkIsBracketOpen, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTOPAR, tkIsComma, NONACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
        {STATE_PROTCPAR, tkIsParenClose, ACCEPTING,
         NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 23: we found a def
    {
        {STATE_DEFID, tkIsIdOrInit, NONACCEPTING, NO_DEF_ID, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 24: we found a def id
    {
        {STATE_DEFPAROP, tkIsParenOpen, NONACCEPTING, INVALID_DEF_PAREN_OPEN,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 25: we're inside the param
    {
        {STATE_DEFTYPEORREF, tkIsTypeOrRef, NONACCEPTING, NO_ERROR,
         NON_CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DEFPARCLO, tkIsParenClose, NONACCEPTING, INVALID_DEF_PAREN_CLOSE,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 26: we're evaluating the param type/ref
    {
        {STATE_DEFTYPE, tkIsType, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DEFREF, tkIsRef, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 27: we closed the def
    {
        {STATE_VALID_START, tkIsConstOrType, NONACCEPTING,
         NO_DEF_VALID_TOKEN_AFTER_PAREN, NON_CONSUMING, NO_SET_CHECKPOINT,
         STACK_STATE},
        {STATE_DEFEND, tkIsEndp, ACCEPTING, NO_DEF_VALID_TOKEN_AFTER_PAREN,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_CMD, tkIsCmdStarter, NONACCEPTING,
         NO_DEF_VALID_TOKEN_AFTER_PAREN, NON_CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 28: def end: accepting
    {},
    // 29: we found a ref
    {
        {STATE_DEFTYPE, tkIsType, NONACCEPTING, NO_DEF_TYPE_AFTER_REF,
         CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 30: we have a type here
    {
        {STATE_DEFPARAMID, tkIsId, NONACCEPTING, NO_DEF_PARAM_ID, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 31: deciding what we want to do after id
    {
        {STATE_DEFBROPEN, tkIsBracketOpen, NONACCEPTING,
         NO_DEF_VALID_TOKEN_AFTER_ID, CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DEFTYPEORREF, tkIsComma, NONACCEPTING,
         NO_DEF_VALID_TOKEN_AFTER_ID, CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_DEFPARCLO, tkIsParenClose, NONACCEPTING,
         NO_DEF_VALID_TOKEN_AFTER_ID, CONSUMING, NO_SET_CHECKPOINT, DONT_STACK},
    },
    // 32: found array bracket open
    {
        {STATE_DEFSUBS, tkIsIntconOrId, NONACCEPTING,
         INVALID_ARRAY_DEF_PARAM_SUBSCRIPT_TYPE, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 33: we want to close this array declaration
    {
        {STATE_DEFPARAMID, tkIsId, NONACCEPTING,
         INVALID_ARRAY_DEF_PARAM_BRACKET_OPEN, CONSUMING, NO_SET_CHECKPOINT,
         DONT_STACK},
    },
    // 34: finally, the cmd state. should go back to cmd for repetition
    {
        {STATE_DEFEND, tkIsEndp, ACCEPTING, NO_DEF_END_KEYWORD, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
        {STATE_GETOUT, tkIsGetout, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISGETINT, tkIsGetint, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISGETREAL, tkIsGetreal, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISGETCHAR, tkIsGetchar, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISGETSTR, tkIsGetstr, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISPUTINT, tkIsPutint, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISPUTREAL, tkIsPutreal, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISPUTCHAR, tkIsPutchar, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
        {STATE_ISPUTSTR, tkIsPutstr, NONACCEPTING, NO_ERROR, CONSUMING,
         NO_SET_CHECKPOINT, STACK_STATE},
    },
    // 35: getout - accepting
    {
        {},
    },
    // 36: isgetint
    {
        {STATE_GETINT, tkIsId, ACCEPTING, NO_GETINT_ID, CONSUMING,
         NO_SET_CHECKPOINT, DONT_STACK},
    },
};

enum SYNTAX_ERROR parse(struct Parser *parser) {
  enum STATE_ALIAS initialState = STATE_INITIAL;

  // the parser starts with an initial consumed token, file and line count
  enum STATE_ALIAS currentState = STATE_INITIAL;
  bool currentIsAccepting = false;
  int stackedState = -1;

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
        if (possibility->setsStackedState) {
          stackedState = currentState;
        }
        currentState = possibility->targetState;
        currentIsAccepting = possibility->isAccepting;
        transitionFound = true;

        if (possibility->setsCheckpoint) {
          initialState = possibility->targetState;
        }
        break;
      }
    }

    if (!transitionFound) {
      if (currentIsAccepting) {
        if (stackedState != -1) {
          currentState = stackedState;
          stackedState = -1;
          continue;
        }
        currentState = initialState;
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
