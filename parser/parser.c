#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/transition.h"
#include "syntax_error.h"

#define MAX_ARRAY_DIMENSIONS 2

enum SYNTAX_ERROR op_rel(struct Parser *parser) {
  if (parser->token.category != SIGN ||
      !(parser->token.signCode == COMPARISON ||
        parser->token.signCode == DIFFERENT ||
        parser->token.signCode == SMALLER_EQUAL ||
        parser->token.signCode == SMALLER_EQUAL ||
        parser->token.signCode == LARGER_EQUAL ||
        parser->token.signCode == LARGER_THAN)) {
    return INVALID_OPERATOR;
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR fator(struct Parser *parser) {
  if (!(parser->token.category == ID || parser->token.category == INTCON ||
        parser->token.category == REALCON ||
        parser->token.category == CHARCON ||
        (parser->token.category == SIGN &&
         parser->token.signCode == OPEN_PAR) ||
        (parser->token.category == SIGN &&
         parser->token.signCode == NEGATION))) {
    return NO_FACTOR_VALID_START_SYMBOL;
  }

  // no need to validate int/real/char?

  // !fator
  if (parser->token.category == SIGN && parser->token.signCode == NEGATION) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = fator(parser);
    if (error != NO_ERROR) {
      return NO_FACTOR_AFTER_BANG;
    }
  }

  // (expr)
  if (parser->token.category == SIGN && parser->token.signCode == OPEN_PAR) {
    enum SYNTAX_ERROR error = expr(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  // id {[expr]}
  if (parser->token.category == ID) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (parser->token.category == SIGN &&
        parser->token.signCode == OPEN_BRACK) {
      enum SYNTAX_ERROR error = arrayFator(parser);
      if (error != NO_ERROR) {
        return error;
      }
    } else if (parser->token.category == SIGN &&
               parser->token.signCode == CLOSE_BRACK) {
      return INVALID_FACTOR_ARRAY_BRACKET_OPEN;
    } else {
      return NO_ERROR;
    }
  }
  return NO_ERROR;
}

enum SYNTAX_ERROR arrayFator(struct Parser *parser) {
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = expr(parser);
  if (error != NO_ERROR) {
    return error;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN &&
        parser->token.signCode == CLOSE_BRACK)) {
    return INVALID_FACTOR_ARRAY_BRACKET_CLOSE;
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR expr(struct Parser *parser) {
  // TODO:
  return NO_ERROR;
}

/**
 * prog accepts repetitions of declarations of variables (decl_list_var), or
 * procedures (decl_list_proc).
 */
enum SYNTAX_ERROR prog(struct Parser *parser) {
  // Both declaration of variables and procedures start with reserved words.
  if (parser->token.category == RSV) {
    // Valid variable declaration start tokens
    while (parser->token.signCode == CONST || parser->token.signCode == CHAR ||
           parser->token.signCode == INT || parser->token.signCode == REAL ||
           parser->token.signCode == BOOL) {
      enum SYNTAX_ERROR error = declListVar(parser);
      if (error != NO_ERROR) {
        return error;
      }
    }
    // Valid procedure declaration/definition tokens
    while (parser->token.signCode == DEF || parser->token.signCode == PROT) {
      enum SYNTAX_ERROR error = declDefProc(parser);
      if (error != NO_ERROR) {
        return error;
      }
    }
  } else {
    return INVALID_PROG_START_KEYWORD;
  }
  return NO_ERROR;
}

/** decl_list_var accepts optionally a `const`, followed by variable type,
 * and
declaration of one or more variables.*/
enum SYNTAX_ERROR declListVar(struct Parser *parser) {
  if (parser->token.signCode == CONST) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (!(parser->token.category == RSV &&
        (parser->token.signCode == INT || parser->token.signCode == REAL ||
         parser->token.signCode == CHAR || parser->token.signCode == BOOL))) {
    return INVALID_TYPE;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = declVar(parser);
  if (error != NO_ERROR) {
    return error;
  }
  // TODO: handle multiple variable declarations here
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  while (parser->token.category == SIGN && parser->token.signCode == COMMA) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = declVar(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }
  return NO_ERROR;
}

enum SYNTAX_ERROR declVar(struct Parser *parser) {
  if (parser->token.category != ID) {
    return NO_VAR_ID;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  bool isArray = false;
  int arrayDimensions = 0;
  // is array
  if (parser->token.category == SIGN && parser->token.signCode == OPEN_BRACK) {
    isArray = true;
    while (parser->token.category == SIGN &&
           parser->token.signCode == OPEN_BRACK) {
      arrayDimensions++;
      if (arrayDimensions > MAX_ARRAY_DIMENSIONS) {
        return INVALID_ARRAY_DIMENSION_DECLARATION;
      }
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);

      if (!(parser->token.category == INTCON || parser->token.category == ID)) {
        return INVALID_ARRAY_SUBSCRIPT_DEC;
      }

      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == SIGN &&
            parser->token.signCode == CLOSE_BRACK)) {
        return INVALID_ARRAY_BRACKET_DEC_CLOSE;
      } else {
        parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      }
    }
  }

  // assignment
  if (parser->token.category == SIGN && parser->token.signCode == ASSIGN) {
    if (isArray) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == SIGN &&
            parser->token.signCode == OPEN_CURLY)) {
        return INVALID_ARRAY_INIT_CURLY_OPEN;
      }

      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);

      while (true) {
        if (!(parser->token.category == INTCON ||
              parser->token.category == REALCON ||
              parser->token.category == CHARCON)) {
          return INVALID_ARRAY_TYPE_INIT;
        }

        parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
        if (parser->token.category == SIGN && parser->token.signCode == COMMA) {
          parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
          continue;

        } else if (parser->token.category == SIGN &&
                   parser->token.signCode == CLOSE_CURLY) {
          break;

        } else {
          return INVALID_ARRAY_INIT_CURLY_CLOSE;
        }
      }

    } else {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == INTCON ||
            parser->token.category == REALCON ||
            parser->token.category == CHARCON)) {
        return INVALID_VAR_TYPE_INIT;
      }
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR declDefProc(struct Parser *parser) {
  bool isDef = false;
  if (parser->token.signCode == DEF) {
    isDef = true;
  }
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);

  if (isDef) {
    if (!(parser->token.category == ID || parser->token.signCode == INIT)) {
      return NO_DEF_ID;
    }
    // is prot
  } else {
    enum SYNTAX_ERROR error = declProt(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR declProt(struct Parser *parser) {
  if (!(parser->token.category == ID)) {
    return NO_PROTO_ID;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN && parser->token.signCode == OPEN_PAR)) {
    return INVALID_PROTO_PAREN_OPEN;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = declProtParam(parser);
  if (error != NO_ERROR) {
    return error;
  }

  if (!(parser->token.category == SIGN &&
        parser->token.signCode == CLOSE_PAR)) {
    return NO_FUNCTION_END_PAREN_CLOSE;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  return NO_ERROR;
}

enum SYNTAX_ERROR declProtParam(struct Parser *parser) {

  if (parser->token.category == SIGN && parser->token.signCode == REF) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (!(parser->token.signCode == CHAR || parser->token.signCode == INT ||
        parser->token.signCode == REAL || parser->token.signCode == BOOL)) {
    return INVALID_PROTO_PARAM_TYPE;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN &&
        (parser->token.signCode == OPEN_BRACK ||
         parser->token.signCode == COMMA ||
         parser->token.signCode == CLOSE_PAR))) {
    return NO_PROTO_VALID_TOKEN_AFTER_TYPE;
  }

  if (parser->token.category == SIGN && parser->token.signCode == OPEN_BRACK) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == SIGN &&
          parser->token.signCode == CLOSE_BRACK)) {
      return INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE;
    }

    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == SIGN &&
          (parser->token.signCode == OPEN_BRACK ||
           parser->token.signCode == COMMA ||
           parser->token.signCode == CLOSE_PAR))) {
      return NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE;
    }

    if (parser->token.category == SIGN &&
        parser->token.signCode == OPEN_BRACK) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == SIGN &&
            parser->token.signCode == CLOSE_BRACK)) {
        return INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE;
      }

      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == SIGN &&
            (parser->token.signCode == COMMA ||
             parser->token.signCode == CLOSE_PAR))) {
        return INVALID_ARRAY_DIMENSION_DECLARATION;
      }
    }

    if (parser->token.category == SIGN && parser->token.signCode == COMMA) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      enum SYNTAX_ERROR error = declProtParam(parser);
      if (error != NO_ERROR) {
        return error;
      }
    }
  }
  return NO_ERROR;
}
