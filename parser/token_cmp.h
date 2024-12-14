#ifndef TOKEN_CMP_H
#define TOKEN_CMP_H

#include <stdbool.h>
#include <stdarg.h>
#include "../lexer/types.h"
#include "./parser.h"

void consumeTokenFrom(struct Parser *parser);

bool tokenCategoryMatchAll(struct Token *token , int matchCount, ...);

bool tokenSignCodeMatchAny(struct Token *token, int matchCount, ...);

#endif
