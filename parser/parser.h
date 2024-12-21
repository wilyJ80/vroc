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

enum SETS_CHECKPOINT_STATE { NO_SET_CHECKPOINT, SET_CHECKPOINT };

enum SETS_STACKED_STATE { DONT_STACK, STACK_STATE };

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
  STATE_ARRCURLCLOSE,
  // 14
  STATE_FUNCTION,
  // 15
  STATE_PROT,
  // 16
  STATE_PROTID,
  // 17
  STATE_PROTOPAR,
  // 18
  STATE_PROTCPAR,
  // 19
  STATE_PROTREF,
  // 20
  STATE_PROTTYPE,
  // 21
  STATE_PROTOBRACK,
  // 22
  STATE_PROTCBRACK,
  // 23
  STATE_DEF,
  // 24
  STATE_DEFID,
  // 25
  STATE_DEFPAROP,
  // 26
  STATE_DEFTYPEORREF,
  // 27
  STATE_DEFPARCLO,
  // 28
  STATE_DEFEND,
  // 29
  STATE_DEFREF,
  // 30
  STATE_DEFTYPE,
  // 31
  STATE_DEFPARAMID,
  // 32
  STATE_DEFBROPEN,
  // 33
  STATE_DEFSUBS,
  // 34
  STATE_CMD,
  // 35
  STATE_GETOUT,
  // 36
  STATE_ISGETINT,
  // 37
  STATE_GETINT,
  // 38
  STATE_ISGETREAL,
  // 39
  STATE_GETREAL,
  // 40
  STATE_ISGETCHAR,
  // 41
  STATE_GETCHAR,
  // 42
  STATE_ISGETSTR,
  // 43
  STATE_GETSTR,
  // 44
  STATE_ISPUTINT,
  // 45
  STATE_PUTINT,
  // 46
  STATE_ISPUTREAL,
  // 47
  STATE_PUTREAL,
  // 48
  STATE_ISPUTCHAR,
  // 49
  STATE_PUTCHAR,
  // 50
  STATE_ISPUTSTR,
  // 51
  STATE_PUTSTR,
};

struct ParserTransition {
  enum STATE_ALIAS targetState;
  bool (*matchFn)(struct Token);
  enum IS_ACCEPTING isAccepting;
  enum SYNTAX_ERROR error;
  enum IS_CONSUMING isConsuming;
  enum SETS_CHECKPOINT_STATE setsCheckpoint;
  enum SETS_STACKED_STATE setsStackedState;
};

enum SYNTAX_ERROR parse(struct Parser *parser);

#endif
