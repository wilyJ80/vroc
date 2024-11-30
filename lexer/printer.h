#ifndef PRINTER_H
#define PRINTER_H

#include "./types.h"

struct TokenCategoryHandler {
  int category;
  void (*tokenPrinterCallback)(struct Token);
};

void handleRsv(struct Token token);

void handleId(struct Token token);

void handleCharcon(struct Token token);

void handleStringcon(struct Token token);

void handleIntcon(struct Token token);

void handleRealcon(struct Token token);

// Sign is a different case altogether... handle it below

void handleSign(struct Token token);

struct SignCategoryHandler {
  int signCategory;
  const char *signCategoryLabel;
};

void printToken(struct Token token);

#endif // !PRINTER_H
