#ifndef TOKEN_MATCH_H
#define TOKEN_MATCH_H

#include "../lexer/types.h"
#include <stdbool.h>

bool tkIsConst(struct Token token);
bool tkIsType(struct Token token);
bool tkIsId(struct Token token);
bool tkIsAssign(struct Token token);
bool tkIsBracketOpen(struct Token token);
bool tkIsIntconRealconOrCharcon(struct Token token);
bool tkIsIntconOrId(struct Token token);
bool tkIsBracketClose(struct Token token);
bool tkIsCurlyOpen(struct Token token);
bool tkIsComma(struct Token token);
bool tkIsCurlyClose(struct Token token);
bool tkIsConstOrType(struct Token token);
bool tkIsProtOrDef(struct Token token);
bool tkIsProt(struct Token token);
bool tkIsParenOpen(struct Token token);
bool tkIsParenClose(struct Token token);
bool tkIsRef(struct Token token);

bool tkIsDef(struct Token token);
bool tkIsIdOrInit(struct Token token);
bool tkIsTypeOrRef(struct Token token);
bool tkIsEndp(struct Token token);
bool tkIsCmdStarter(struct Token token);

bool tkIsGetout(struct Token token);

bool tkIsGetint(struct Token token);
bool tkIsGetreal(struct Token token);
bool tkIsGetchar(struct Token token);
bool tkIsGetstr(struct Token token);

bool tkIsPutint(struct Token token);
bool tkIsPutreal(struct Token token);
bool tkIsPutchar(struct Token token);
bool tkIsPutstr(struct Token token);

#endif
