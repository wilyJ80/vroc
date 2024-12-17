#ifndef TOKEN_MATCH_H
#define TOKEN_MATCH_H

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

#endif
