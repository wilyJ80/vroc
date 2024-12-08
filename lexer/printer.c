#include "printer.h"
#include "transition.h"
#include "types.h"
#include <stdio.h>

#define TOKEN_CATEGORY_QTY 7
#define SIGN_CATEGORY_QTY 22

#define KEYWORD_QTY 31

struct TokenCategoryHandler tokenCategoryHandleData[TOKEN_CATEGORY_QTY] = {
    {RSV, handleRsv},         {ID, handleId},
    {CHARCON, handleCharcon}, {STRINGCON, handleStringcon},
    {INTCON, handleIntcon},   {REALCON, handleRealcon},
    {SIGN, handleSign}};

struct SignCategoryHandler signCategoryHandleData[SIGN_CATEGORY_QTY] = {
    {ASSIGN, "ASSIGN"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {STAR, "STAR"},
    {SLASH, "SLASH"},
    {OPEN_PAR, "OPEN_PAR"},
    {CLOSE_PAR, "CLOSE_PAR"},
    {OPEN_BRACK, "OPEN_BRACK"},
    {CLOSE_BRACK, "CLOSE_BRACK"},
    {COMMA, "COMMA"},
    {COMPARISON, "COMPARISON"},
    {REF, "REF"},
    {AND, "AND"},
    {OR, "OR"},
    {SMALLER_THAN, "SMALLER_THAN"},
    {LARGER_THAN, "LARGER_THAN"},
    {NEGATION, "NEGATION"},
    {DIFFERENT, "DIFFERENT"},
    {SMALLER_EQUAL, "SMALLER_EQUAL"},
    {LARGER_EQUAL, "LARGER_EQUAL"},
    {OPEN_CURLY, "OPEN_CURLY"},
    {CLOSE_CURLY, "CLOSE_CURLY"}};

void handleRsv(struct Token token) {

  struct ReservedWord reservedKeywords[KEYWORD_QTY] = {
      {"const", CONST},   {"init", INIT},       {"endp", ENDP},
      {"char", CHAR},     {"int", INT},         {"real", REAL},
      {"bool", BOOL},     {"do", DO},           {"while", WHILE},
      {"endw", ENDW},     {"var", VAR},         {"from", FROM},
      {"to", TO},         {"dt", DT},           {"by", BY},
      {"if", IF},         {"endv", ENDV},       {"elif", ELIF},
      {"else", ELSE},     {"endi", ENDI},       {"getout", GETOUT},
      {"getint", GETINT}, {"getchar", GETCHAR}, {"getreal", GETREAL},
      {"putint", PUTINT}, {"putchar", PUTCHAR}, {"putreal", PUTREAL},
      {"getstr", GETSTR}, {"putstr", PUTSTR},   {"def", DEF},
      {"prot", PROT}};

  for (int i = 0; i < KEYWORD_QTY; i++) {
    if (token.signCode == reservedKeywords[i].reservedCode) {
      printf("<RSV, %s>", reservedKeywords[i].lexeme);
    }
  }
}

void handleId(struct Token token) { printf("<ID, %s>", token.lexeme); }

void handleCharcon(struct Token token) {
  printf("<CHARCON, %s>", token.lexeme);
}

void handleStringcon(struct Token token) {
  printf("<STRINGCON, %s>", token.lexeme);
}

void handleIntcon(struct Token token) {
  printf("<INTCON, %d>", token.intValue);
}

void handleRealcon(struct Token token) {
  printf("<REALCON, %lf>", token.doubleValue);
}

void handleSign(struct Token token) {
  for (int i = 0; i < SIGN_CATEGORY_QTY; i++) {
    if (token.signCode == signCategoryHandleData[i].signCategory) {
      printf("<SN, %s>", signCategoryHandleData[i].signCategoryLabel);
    }
  }
}

void printToken(struct Token token) {
  for (int i = 0; i < TOKEN_CATEGORY_QTY; i++) {
    if (token.category == tokenCategoryHandleData[i].category) {
      tokenCategoryHandleData[i].tokenPrinterCallback(token);
    }
  }
}
