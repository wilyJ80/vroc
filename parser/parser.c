#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/transition.h"
#include "syntax_error.h"
#include "token_cmp.h"
#include "token_match.h"

#define MAX_STATES 12
#define MAX_TRANSITIONS 2

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES + 1][MAX_TRANSITIONS] = {
    // 0: initial: accepting
    {
        {STATE_CONST, tkIsConst, NONACCEPTING, NO_ERROR},
        {STATE_DLV, tkIsType, NONACCEPTING, INVALID_TYPE},
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
  while (parser->token.category != END_OF_FILE) {

    // the parser starts with an initial consumed token, file and line count
    enum STATE_ALIAS currentState = STATE_INITIAL;
    bool currentIsAccepting = true;
    enum SYNTAX_ERROR currentError;

    for (int possibleTransition = 0;
         possibleTransition < MAX_TRANSITIONS + 1 &&
         possibleTransitions[currentState][possibleTransition].matchFn != NULL;
         possibleTransition++) {

      struct ParserTransition *possibility =
          &possibleTransitions[currentState][possibleTransition];

      if (possibility->matchFn(parser->token)) {
        // advance fsm
        currentState = possibility->targetState;
        currentIsAccepting = possibility->isAccepting;

      } else {
        // check accepting: errors are optional
        if (currentIsAccepting) {
          return NO_ERROR;
        } else {
          return possibility->error;
        }
      }
    }
  }

  return NO_ERROR;
}
