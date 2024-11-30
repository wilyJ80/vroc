#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_SIZE_LEXEME 31

enum TOKEN_CAT {
  ID,
  SIGN,
  END_EXPR,
  END_OF_FILE,
  CHARCON,
  STRINGCON,
  INTCON,
  REALCON,
  NON_ACCEPTING,
  COMMENT_IGNORE,
  MALFORMED_TOKEN,
  RSV
};

enum SIGN {
  NOT_SYMBOL,
  ASSIGN,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  OPEN_PAR,
  CLOSE_PAR,
  OPEN_BRACK,
  CLOSE_BRACK,
  COMMA,
  COMPARISON,
  REF,
  AND,
  OR,
  SMALLER_THAN,
  LARGER_THAN,
  NEGATION,
  DIFFERENT,
  SMALLER_EQUAL,
  LARGER_EQUAL
};

enum IS_OTHER { IS_OTHER, NOT_OTHER };

struct Token {
  enum TOKEN_CAT category;
  union {
    int signCode;
    char lexeme[MAX_SIZE_LEXEME];
    int intValue;
    double doubleValue;
  };
};

struct Transition {
  int nextState;
  bool (*charMatch)(char);
  enum TOKEN_CAT category;
  enum IS_OTHER isOther;
  enum SIGN signCode;
};

#endif // !TYPES_H
