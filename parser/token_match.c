#include "token_match.h"
#include "../lexer/transition.h"
#include "../lexer/types.h"

#include <stdbool.h>

bool tkIsConst(struct Token token) { return token.signCode == CONST; }

bool tkIsType(struct Token token) {
  return (token.category == RSV &&
          (token.signCode == INT || token.signCode == CHAR ||
           token.signCode == REAL || token.signCode == BOOL));
}

bool tkIsId(struct Token token) { return token.category == ID; }

bool tkIsAssign(struct Token token) {
  return (token.category == SIGN && token.signCode == ASSIGN);
}

bool tkIsBracketOpen(struct Token token) {
  return (token.category == SIGN && token.signCode == OPEN_BRACK);
}

bool tkIsIntconRealconOrCharcon(struct Token token) {
  return (token.category == INTCON || token.category == REALCON ||
          token.category == CHARCON);
}

bool tkIsIntconOrId(struct Token token) {
  return (token.category == ID || token.category == INTCON);
}

bool tkIsBracketClose(struct Token token) {
  return (token.category == SIGN && token.signCode == CLOSE_BRACK);
}

bool tkIsCurlyOpen(struct Token token) {
  return (token.category == SIGN && token.signCode == OPEN_CURLY);
}

bool tkIsComma(struct Token token) {
  return (token.category == SIGN && token.signCode == COMMA);
}

bool tkIsCurlyClose(struct Token token) {
  return (token.category == SIGN && token.signCode == CLOSE_CURLY);
}

bool tkIsConstOrType(struct Token token) {
  return (token.category == RSV &&
          (token.signCode == CONST || token.signCode == INT ||
           token.signCode == REAL || token.signCode == CHAR ||
           token.signCode == BOOL));
}

bool tkIsProtOrDef(struct Token token) {
  return (token.category == RSV &&
          (token.signCode == PROT || token.signCode == DEF));
}

bool tkIsProt(struct Token token) {
  return (token.category == RSV && token.signCode == PROT);
}

bool tkIsParenOpen(struct Token token) {
  return (token.category == SIGN && token.signCode == OPEN_PAR);
}

bool tkIsParenClose(struct Token token) {
  return (token.category == SIGN && token.signCode == CLOSE_PAR);
}

bool tkIsRef(struct Token token) {
  return (token.category == SIGN && token.signCode == REF);
}

bool tkIsIdOrInit(struct Token token) {
  return (token.category == ID ||
          (token.category == RSV && token.signCode == INIT));
}

bool tkIsTypeOrRef(struct Token token) {
  return ((token.category == SIGN && token.signCode == REF) ||
          token.category == RSV &&
              (token.signCode == INT || token.signCode == CHAR ||
               token.signCode == REAL || token.signCode == BOOL));
  ;
}

bool tkIsEndp(struct Token token) {
  return (token.category == RSV && token.signCode == ENDP);
}

bool tkIsCmdStarter(struct Token token) {
  return (token.category == ID ||
          (token.category == RSV &&
           (token.signCode == GETOUT || token.signCode == GETINT ||
            token.signCode == GETREAL || token.signCode == GETCHAR ||
            token.signCode == GETSTR || token.signCode == PUTINT ||
            token.signCode == PUTREAL || token.signCode == PUTCHAR ||
            token.signCode == PUTSTR)));
}

bool tkIsDef(struct Token token) {
  return (token.category == RSV && token.signCode == DEF);
}
