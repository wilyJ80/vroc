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

enum IS_ACCEPTING { NONACCEPTING, ACCEPTING };

enum IS_OPTIONAL { NOT_OPTIONAL, OPTIONAL };

enum IS_CONSUMING { NON_CONSUMING, CONSUMING };

enum STATE_ALIAS {
  // 0
  STATE_INITIAL,
  // 1
  STATE_VALID_START,
  // 2
  STATE_CONST,
  // 3
  STATE_DLV,
  // 4
  STATE_DV,
  // 5
  STATE_ASS,
  // 6
  STATE_ASSEND,
  // 7
  STATE_ARROPEN,
  // 8
  STATE_SUBS,
  // 9
  STATE_ARRCLOSE,
  // 10
  STATE_ARRASS,
  // 11
  STATE_ARRCURLYO,
  // 12
  STATE_ARRINITTYPE,
  // 13
  STATE_ARRCURLCLOSE
};

struct ParserTransition {
  enum STATE_ALIAS targetState;
  bool (*matchFn)(struct Token);
  enum IS_ACCEPTING isAccepting;
  enum SYNTAX_ERROR error;
  enum IS_CONSUMING isConsuming;
};

enum SYNTAX_ERROR parse(struct Parser *parser);

#endif
