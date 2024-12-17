#ifndef TOKEN_MATCH_H
#define TOKEN_MATCH_H

bool isConst(struct Token token);
bool isType(struct Token token);
bool isId(struct Token token);
bool isAssign(struct Token token);
bool isBracketOpen(struct Token token);
bool isIntconRealconOrCharcon(struct Token token);
bool isIntconOrId(struct Token token);

#endif
