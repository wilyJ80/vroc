#include "char.h"
#include <ctype.h>

bool isNonPrinting(char c) { return !isprint((unsigned char)c); }

bool isAlpha(char c) { return isalpha((unsigned char)c); }

bool isDigit(char c) { return isdigit((unsigned char)c); }

bool isAlnumOrUnderscore(char c) {
  return isalnum((unsigned char)c) || c == '_';
}

bool isNotAlnumOrUnderscore(char c) { return !isAlnumOrUnderscore(c); }

bool isNotDigit(char c) { return !isDigit(c); }

bool isPlus(char c) { return c == '+'; }

bool isMinus(char c) { return c == '-'; }

bool isSlash(char c) { return c == '/'; }

bool isStar(char c) { return c == '*'; }

bool isEqual(char c) { return c == '='; }

bool isParenOpen(char c) { return c == '('; }

bool isParenClose(char c) { return c == ')'; }

bool isUnderscore(char c) { return c == '_'; }

bool isSingleQuote(char c) { return c == '\''; }

bool isDoubleQuote(char c) { return c == '"'; }

bool isBracketOpen(char c) { return c == '['; }

bool isBracketClose(char c) { return c == ']'; }

bool isComma(char c) { return c == ','; }

bool isRef(char c) { return c == '&'; }

bool isPipe(char c) { return c == '|'; }

bool isSmallerThan(char c) { return c == '<'; }

bool isLargerThan(char c) { return c == '>'; }

bool isNeg(char c) { return c == '!'; }

bool isNotAlpha(char c) { return !isAlpha(c); }

bool isPeriod(char c) { return c == '.'; }

bool isIsPrint(char c) { return isprint(c); }

bool isBackSlash(char c) { return c == '\\'; }
