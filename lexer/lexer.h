#ifndef LEXER_H
#define LEXER_H

#include "../types/types.h"

struct Token lexerGetNextChar(FILE *fd, int *lineCount);

#endif // !LEXER_H
