#include "token_cmp.h"
#include "../lexer/types.h"
#include <stdarg.h>
#include "./parser.h"
#include "../lexer/lexer.h"

void consumeTokenFrom(struct Parser *parser) {
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
}

bool tokenCategoryMatchAll(struct Parser *parser, int matchCount, ...) {
  va_list va;
  va_start(va, matchCount);

  for (int i = 0; i < matchCount; i++) {
    if (parser->token.category != va_arg(va, int)) {
      va_end(va);
      return false;
    }
  }

  va_end(va);
  return true;
}

bool tokenSignCodeMatchAny(struct Parser *parser, int matchCount, ...) {
  va_list va;
  va_start(va, matchCount);
  bool foundOne = false;

  for (int i = 0; i < matchCount; i++) {
    if (parser->token.signCode == va_arg(va, int)) {
      foundOne = true;
    }
  }

  va_end(va);
  return foundOne;
}
