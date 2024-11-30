#ifndef TRANSITION_H
#define TRANSITION_H

#include "./types.h"
#include <stdbool.h>
#include <stdio.h>

bool handleTransitionAndWasTokenBuilt(FILE *fd, char ch, struct Token *token,
                                      char *lexeme, int *lexemeSize,
                                      struct Transition transition,
                                      int *lineCount, int state);

enum RESERVED {
  CONST,
  INIT,
  ENDP,
  CHAR,
  INT,
  REAL,
  BOOL,
  DO,
  WHILE,
  ENDW,
  VAR,
  FROM,
  TO,
  DT,
  BY,
  IF,
  ENDV,
  ELIF,
  ELSE,
  ENDI,
  GETOUT,
  GETINT,
  GETCHAR,
  GETREAL,
  PUTINT,
  PUTCHAR,
  PUTREAL,
  GETSTR,
  PUTSTR,
  DEF,
  PROT
};

struct ReservedWord {
  const char *lexeme;
  enum RESERVED reservedCode;
};

#endif // !TRANSITION_H
