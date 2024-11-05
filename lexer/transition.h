#ifndef TRANSITION_H
#define TRANSITION_H

#include <stdio.h>
#include <stdbool.h>
#include "../types/types.h"

bool handleTransitionAndWasTokenBuilt(FILE *fd, char ch, struct Token *token, char *lexeme,
                      int *lexemeSize, struct Transition transition, int lineCount, int state);

#endif // !TRANSITION_H
