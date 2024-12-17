#include "parser.h"
#include "../lexer/transition.h"
#include "syntax_error.h"
#include "token_cmp.h"
#include "token_match.h"

#define MAX_STATES 10
#define MAX_TRANSITIONS 2

// No error means transition is optional.
// Optional transition: keep traversing to find next errors.
struct ParserTransition possibleTransitions[MAX_STATES][MAX_TRANSITIONS] = {
    // 0: initial: accepting
    {
        {STATE_CONST, isConst, NONACCEPTING, NO_ERROR},
        {STATE_DLV, isType, NONACCEPTING, INVALID_TYPE},
    },
    // 1: const
    {
        {STATE_DLV, isType, NONACCEPTING, INVALID_TYPE},
    },
    // 2: decl list var
    {
        {STATE_DV, isId, ACCEPTING, NO_VAR_ID},
    },
    // 3: decl var: accepting
    {
        {STATE_ASS, isAssign, NONACCEPTING, NO_ERROR},
        {STATE_ARROPEN, isBracketOpen, NONACCEPTING, NO_ERROR},
    },
    // 4: assign single
    {
        {STATE_ASSEND, isIntconRealconOrCharcon, ACCEPTING,
         INVALID_VAR_TYPE_INIT},
    },
    // 5: terminal - assigned variable ok
    {},
    // 6: start array declaration
    {
        {STATE_SUBS, isIntconOrId, NONACCEPTING, INVALID_ARRAY_SUBSCRIPT_DEC},
    },
};
