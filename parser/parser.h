#ifndef PARSER_H
#define PARSER_H

#include "../lexer/types.h"
#include "./syntax_error.h"
#include <stdio.h>

struct Parser {
  struct Token token;
  FILE *fd;
  int *lineCount;
};

enum IS_ACCEPTING {
  ACCEPTING, NONACCEPTING
};

enum IS_OPTIONAL {
  OPTIONAL, NOT_OPTIONAL
};

struct ParserTransition {
  int targetState;
  bool (*matchFn)(struct Token);
  enum IS_ACCEPTING isAccepting;
  enum SYNTAX_ERROR error;
  enum IS_OPTIONAL isOptional;
};

enum STATE_ALIAS {
  // 0
  STATE_INITIAL,
  // 1
  STATE_CONST,
  // 2
  STATE_DLV,
  // 3
  STATE_DV,
  // 4
  STATE_ASS,
  // 5
  STATE_ASSEND,
  // 6
  STATE_ARROPEN,
  // 7
  STATE_SUBS,
  // 8
  STATE_ARRCLOSE,
  // 9
  STATE_ARRASS,
  // 10
  STATE_ARRCURLYO,
  // 11
  STATE_ARRINITTYPE,
  // 12
  STATE_ARRCURLCLOSE
};

#endif
