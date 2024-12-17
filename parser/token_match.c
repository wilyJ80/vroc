#include "token_match.h"
#include "../lexer/transition.h"
#include "../lexer/types.h"

bool isConst(struct Token token) { return token.signCode == CONST; }

bool isType(struct Token token) {
  return (token.category == RSV &&
          (token.signCode == INT || token.signCode == CHAR ||
           token.signCode == REAL || token.signCode == BOOL));
}

bool isId(struct Token token) { return token.category == ID; }

bool isAssign(struct Token token) {
  return (token.category == SIGN && token.signCode == ASSIGN);
}

bool isBracketOpen(struct Token token) {
  return (token.category == SIGN && token.signCode == OPEN_BRACK);
}

bool isIntconRealconOrCharcon(struct Token token) {
  return (token.category == INTCON || token.category == REALCON ||
          token.category == CHARCON);
}

bool isIntconOrId(struct Token token) {
  return (token.category == ID || token.category == INTCON);
}
