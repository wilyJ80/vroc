#include "printer.h"
#include "types.h"
#include <stdio.h>

#define TOKEN_CATEGORY_QTY 7
#define SIGN_CATEGORY_QTY 22

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

void handleRsv(struct Token token) { printf("<RSV, %s>", token.lexeme); }

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
