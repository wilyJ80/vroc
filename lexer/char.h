#ifndef CHAR_H
#define CHAR_H

#include <stdbool.h>

bool isNonPrinting(char c);
bool isAlpha(char c);
bool isDigit(char c);
bool isAlnumOrUnderscore(char c);
bool isNotAlnumOrUnderscore(char c);
bool isNotDigit(char c);
bool isPlus(char c);
bool isMinus(char c);
bool isSlash(char c);
bool isStar(char c);
bool isEqual(char c);
bool isParenOpen(char c);
bool isParenClose(char c);
bool isUnderscore(char c);
bool isSingleQuote(char c);
bool isDoubleQuote(char c);
bool isBracketOpen(char c);
bool isBracketClose(char c);
bool isComma(char c);
bool isRef(char c);
bool isPipe(char c);
bool isSmallerThan(char c);
bool isLargerThan(char c);
bool isNeg(char c);
bool isNotAlpha(char c);
bool isPeriod(char c);
bool isIsPrint(char c);
bool isBackSlash(char c);
bool isTerminating(char c);
bool isNewline(char c);
bool isNotIsPrintAndIsNeitherNewlineNorTerminating(char c);
bool isNotDoubleQuote(char c);
bool isNotSlash(char c);
bool isNotNewline(char c);
bool isNotEqual(char c);
bool isNotRef(char c);
bool isNotDigitNorPeriod(char c);
bool isZeroOrN(char c);
bool isIsPrintButNotBackSlash(char c);
bool isCurlyOpen(char c);
bool isCurlyClose(char c);

#endif // !CHAR_H
