#include "parser.h"
#include "../lexer/lexer.h"
#include "../lexer/transition.h"
#include "syntax_error.h"
#include "token_cmp.h"

#define MAX_ARRAY_DIMENSIONS 2

/**
 * prog accepts repetitions of declarations of variables (decl_list_var), or
 * procedures (decl_list_proc).
 */
enum SYNTAX_ERROR prog(struct Parser *parser) {
  // Both declaration of variables and procedures start with reserved words.
  // Valid variable declaration start tokens
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         (tokenSignCodeMatchAny(parser, 5, CONST, CHAR, INT, REAL, BOOL))) {
    enum SYNTAX_ERROR error = declListVar(parser);
    if (error) {
      return error;
    }
  }
  // Valid procedure declaration/definition tokens
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 2, DEF, PROT)) {
    enum SYNTAX_ERROR error = declDefProc(parser);
    if (error) {
      return error;
    }
  }

  // anything other than eof
  if (!(tokenCategoryMatchAll(parser, 1, END_OF_FILE))) {
    return INVALID_PROG_START_KEYWORD;
  }

  return NO_ERROR;
}

/** decl_list_var accepts optionally a `const`, followed by variable type,
 * and
declaration of one or more variables.*/
enum SYNTAX_ERROR declListVar(struct Parser *parser) {
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, CONST)) {
    consumeTokenFrom(parser);
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 4, INT, REAL, CHAR, BOOL))) {
    return INVALID_TYPE;
  }

  consumeTokenFrom(parser);

  do {
    // consume comma from subsequent iterations
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, COMMA)) {
      consumeTokenFrom(parser);
    }
    enum SYNTAX_ERROR error = declVar(parser);
    if (error) {
      return error;
    }
  } while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, COMMA));

  return NO_ERROR;
}

enum SYNTAX_ERROR declVar(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, ID))) {
    return NO_VAR_ID;
  }
  consumeTokenFrom(parser);

  // simple variable assignment
  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 1, ASSIGN)) {
    consumeTokenFrom(parser);

    if (!(tokenCategoryMatchAll(parser, 3, INTCON, REALCON, CHARCON))) {
      return INVALID_VAR_TYPE_INIT;
    }

    return NO_ERROR;
  }

  // the following is related to array variable: single variable returned early
  // above
  while (tokenCategoryMatchAll(parser, 1, SIGN) &&
         tokenSignCodeMatchAny(parser, 1, OPEN_BRACK)) {
    consumeTokenFrom(parser);
    if (!(tokenCategoryMatchAll(parser, 2, INTCON, ID))) {
      return INVALID_ARRAY_SUBSCRIPT_DEC;
    }
    consumeTokenFrom(parser);
    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, CLOSE_BRACK))) {
      return INVALID_ARRAY_BRACKET_DEC_CLOSE;
    }
    consumeTokenFrom(parser);
  }

  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 1, ASSIGN)) {
    consumeTokenFrom(parser);
    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, OPEN_CURLY))) {
      return INVALID_ARRAY_INIT_CURLY_OPEN;
    }
    consumeTokenFrom(parser);
  }

  do {
    // consume comma from subsequent iterations
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, COMMA)) {
      consumeTokenFrom(parser);
    }
    enum SYNTAX_ERROR error = declVarArrayInit(parser);
    if (error) {
      return error;
    }
  } while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, COMMA));

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_CURLY))) {
    return INVALID_ARRAY_INIT_CURLY_CLOSE;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR declVarArrayInit(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 3, INTCON, REALCON, CHARCON))) {
    return INVALID_ARRAY_TYPE_INIT;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR declDefProc(struct Parser *parser) {
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, DEF)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = declDef(parser);
    if (error) {
      return error;
    }
  }

  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, PROT)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = declProt(parser);
    if (error) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR declProt(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, ID))) {
    return NO_PROTO_ID;
  }

  consumeTokenFrom(parser);
  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 1, OPEN_PAR)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = declProtParam(parser);
    if (error) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR declProtParam(struct Parser *parser) {
  bool paramMandatory = false;
  bool somethingInside = false;

  do {
    // consume comma from subsequent iterations
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, COMMA)) {
      consumeTokenFrom(parser);
    }
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, REF)) {
      paramMandatory = true;
      consumeTokenFrom(parser);
      somethingInside = true;
    }

    if (paramMandatory &&
        !(tokenCategoryMatchAll(parser, 1, RSV) &&
          tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL))) {
      return NO_PROTO_TYPE_AFTER_REF;
    }

    if (tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL)) {
      consumeTokenFrom(parser);
      somethingInside = true;

      while (tokenCategoryMatchAll(parser, 1, SIGN) &&
             tokenSignCodeMatchAny(parser, 1, OPEN_BRACK)) {
        consumeTokenFrom(parser);

        if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
              tokenSignCodeMatchAny(parser, 1, CLOSE_BRACK))) {
          return INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE;
        }
        consumeTokenFrom(parser);
      }
    }

  } while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, COMMA));

  if (!somethingInside) {
    consumeTokenFrom(parser);
  }
  if (!(tokenCategoryMatchAll(parser, 1, CLOSE_PAR))) {
    return NO_FUNCTION_END_PAREN_CLOSE;
  }
  consumeTokenFrom(parser);

  return NO_ERROR;
}

enum SYNTAX_ERROR declDef(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, ID) ||
        (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 1, INIT)))) {
    return NO_DEF_ID;
  }

  consumeTokenFrom(parser);
  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, OPEN_PAR))) {
    return INVALID_DEF_PAREN_OPEN;
  }

  consumeTokenFrom(parser);
  enum SYNTAX_ERROR error = declDefParam(parser);
  if (error) {
    return error;
  }

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_DEF_PAREN_CLOSE;
  }

  // can be endp, declListVar, or cmd
  consumeTokenFrom(parser);

  // is declListVar
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 5, CONST, CHAR, INT, REAL, BOOL)) {
    enum SYNTAX_ERROR error = declListVar(parser);
    if (error) {
      return error;
    }
  }

  // is cmd
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                            GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL, PUTCHAR,
                            PUTSTR, ID)) {
    enum SYNTAX_ERROR error = cmd(parser);
    if (error) {
      return error;
    }
  }

  // is endp
  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, ENDP))) {
    return NO_DEF_END_KEYWORD;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR declDefParam(struct Parser *parser) {

  // while it's a param, delimited by comma
  do {
    // skip the comma if it's a subsequent param
    if (parser->token.category == SIGN && parser->token.signCode == COMMA) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    }

    // & is optional
    if (parser->token.category == SIGN && parser->token.signCode == REF) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    }

    // type is valid
    if (!(parser->token.category == RSV &&
          (parser->token.signCode == INT || parser->token.signCode == CHAR ||
           parser->token.signCode == REAL || parser->token.signCode == BOOL))) {
      return INVALID_DEF_PARAM_TYPE;
    }

    // param id
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID)) {
      return NO_DEF_PARAM_ID;
    }

    // valid token after id
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == SIGN &&
          (parser->token.signCode == OPEN_BRACK ||
           parser->token.signCode == COMMA ||
           parser->token.signCode == CLOSE_PAR))) {
      return NO_DEF_VALID_TOKEN_AFTER_ID;
    }
    // CLOSE_PAR has no extra steps: simply breaks the loop and leaves

    // OPEN_BRACK defines an array param
    if (parser->token.signCode == OPEN_BRACK) {
      enum SYNTAX_ERROR error = declDefParamArray(parser);
      if (error != NO_ERROR) {
        return error;
      }
    }

    // COMMA loops the whole thing
  } while (parser->token.category == SIGN && parser->token.signCode == COMMA);

  return NO_ERROR;
}

enum SYNTAX_ERROR declDefParamArray(struct Parser *parser) {
  do {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID || parser->token.category == INTCON)) {
      return INVALID_ARRAY_DEF_PARAM_SUBSCRIPT_TYPE;
    }

    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == SIGN &&
          parser->token.signCode == CLOSE_BRACK)) {
      return INVALID_ARRAY_DEF_PARAM_BRACKET_CLOSE;
    }

    // consume next (should be OPEN_BRACK or COMMA)
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  } while (parser->token.signCode == OPEN_BRACK);

  // is COMMA
  return NO_ERROR;
}

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
  // this makes no damn sense: keeping it for unit tests, I guess
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
    return NO_ERROR;
  }

  // (expr)
  if (parser->token.category == SIGN && parser->token.signCode == OPEN_PAR) {
    enum SYNTAX_ERROR error = expr(parser);
    if (error != NO_ERROR) {
      return error;
    }
    if (!(parser->token.category == SIGN &&
          parser->token.signCode == CLOSE_PAR)) {
      return INVALID_FACTOR_EXPR_PAREN_CLOSE;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    return NO_ERROR;
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
  while (parser->token.category == SIGN &&
         parser->token.signCode == OPEN_BRACK) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = expr(parser);
    if (error != NO_ERROR) {
      return error;
    }

    if (!(parser->token.category == SIGN &&
          parser->token.signCode == CLOSE_BRACK)) {
      return INVALID_FACTOR_ARRAY_BRACKET_CLOSE;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR cmd(struct Parser *parser) {
  if (parser->token.signCode == GETINT) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID)) {
      return NO_GETINT_ID;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == GETREAL) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID)) {
      return NO_GETREAL_ID;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == GETCHAR) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID)) {
      return NO_GETCHAR_ID;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == GETSTR) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID)) {
      return NO_GETSTR_ID;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == PUTINT) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID || parser->token.category == INTCON)) {
      return INVALID_PUTINT_ELEMENT;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == PUTREAL) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID || parser->token.category == REALCON)) {
      return INVALID_PUTREAL_ELEMENT;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == PUTCHAR) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID || parser->token.category == CHARCON)) {
      return INVALID_PUTCHAR_ELEMENT;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == PUTSTR) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == ID ||
          parser->token.category == STRINGCON)) {
      return INVALID_PUTSTR_ELEMENT;
    }
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  if (parser->token.signCode == DO) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = cmdDo(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  if (parser->token.signCode == WHILE) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = cmdWhile(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  if (parser->token.signCode == VAR) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = cmdVar(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  if (parser->token.signCode == IF) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = cmdIf(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  if (parser->token.category == ID) {
    enum SYNTAX_ERROR error = cmdAtrib(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  // getout: nothing needed?

  return NO_ERROR;
}

enum SYNTAX_ERROR cmdIf(struct Parser *parser) {
  if (!(parser->token.category == SIGN && parser->token.signCode == OPEN_PAR)) {
    return INVALID_IF_PAREN_OPEN;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = expr(parser);
  if (error) {
    return error;
  }

  if (!(parser->token.category == SIGN &&
        parser->token.signCode == CLOSE_PAR)) {
    return INVALID_IF_PAREN_CLOSE;
  }

  do {

    enum SYNTAX_ERROR error = cmd(parser);
    if (error != NO_ERROR) {
      return error;
    }

  } while (
      !(parser->token.category == RSV &&
        (parser->token.signCode == ELIF || parser->token.signCode == ELSE ||
         parser->token.signCode == ENDI)));

  do {
    if (parser->token.category == RSV && parser->token.signCode == ELIF) {
      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      if (!(parser->token.category == SIGN &&
            parser->token.signCode == OPEN_PAR)) {
        return INVALID_ELIF_PAREN_OPEN;
      }

      parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
      enum SYNTAX_ERROR error = expr(parser);
      if (error != NO_ERROR) {
        return error;
      }

      if (!(parser->token.category == SIGN &&
            parser->token.signCode == CLOSE_PAR)) {
        return INVALID_ELIF_PAREN_CLOSE;
      }

      do {
        enum SYNTAX_ERROR error = cmd(parser);
        if (error != NO_ERROR) {
          return error;
        }
      } while (
          !(parser->token.category == RSV &&
            (parser->token.signCode == ELSE || parser->token.signCode == ELIF ||
             parser->token.signCode == ENDI)));
    }
  } while (parser->token.category == RSV && parser->token.signCode == ELIF);

  return NO_ERROR;
}

enum SYNTAX_ERROR cmdVar(struct Parser *parser) {
  if (!(parser->token.category == ID)) {
    return NO_FOR_ID;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == RSV && parser->token.signCode == FROM)) {
    return NO_FOR_FROM;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == RSV &&
        (parser->token.signCode == TO || parser->token.signCode == DT))) {
    return NO_FOR_TO_OR_DT;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = expr(parser);
  if (error != NO_ERROR) {
    return error;
  }

  // optional by
  if (parser->token.category == RSV && parser->token.signCode == BY) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    if (!(parser->token.category == INTCON || parser->token.category == ID)) {
      return INVALID_FOR_BY_INC_OR_DEC_TYPE;
    }
  }

  // suggested change: do while token is valid cmd starter
  do {

    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error2 = cmd(parser);

  } while (!(parser->token.category == RSV && parser->token.signCode == ENDV));

  return NO_ERROR;
}

enum SYNTAX_ERROR cmdWhile(struct Parser *parser) {
  if (!(parser->token.category == SIGN && parser->token.signCode == OPEN_PAR)) {
    return INVALID_WHILE_PAREN_OPEN;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error = expr(parser);
  if (error != NO_ERROR) {
    return error;
  }

  if (!(parser->token.category == SIGN &&
        parser->token.signCode == CLOSE_PAR)) {
    return INVALID_WHILE_PAREN_CLOSE;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  enum SYNTAX_ERROR error2 = cmd(parser);
  if (error2 != NO_ERROR) {
    return error;
  }

  if (!(parser->token.category == RSV && parser->token.signCode == ENDW)) {
    return NO_WHILE_END_KEYWORD;
  }

  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  return NO_ERROR;
}

enum SYNTAX_ERROR cmdAtrib(struct Parser *parser) {
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN &&
        (parser->token.signCode == OPEN_BRACK ||
         parser->token.signCode == ASSIGN))) {
    return NO_ATRIB_VALID_TOKEN_AFTER_ID;
  }

  if (parser->token.category == SIGN && parser->token.signCode == ASSIGN) {
    enum SYNTAX_ERROR error = fator(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  // arrayAtrib
  while (parser->token.category == SIGN &&
         parser->token.signCode == OPEN_BRACK) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = expr(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR cmdDo(struct Parser *parser) {
  if (!(parser->token.category == ID)) {
    return INVALID_FUNCTION_CALL_ID;
  }

  // is id, ok... then it should open a paren
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN && parser->token.signCode == OPEN_PAR)) {
    return INVALID_FUNCTION_CALL_PAREN_OPEN;
  }

  // do...

  do {
    enum SYNTAX_ERROR error = expr(parser);
    if (error != NO_ERROR) {
      return error;
    }
  } while (parser->token.category == SIGN && parser->token.signCode == COMMA);

  // check paren close
  parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  if (!(parser->token.category == SIGN &&
        parser->token.signCode == CLOSE_PAR)) {
    return INVALID_FUNCTION_CALL_PAREN_CLOSE;
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR expr(struct Parser *parser) {
  bool opRelFound = false;
  enum SYNTAX_ERROR error = exprSimp(parser);
  if (error != NO_ERROR) {
    return error;
  }

  // functions following expr should always carry over the next token!
  if (parser->token.category == SIGN &&
      (parser->token.signCode == COMPARISON ||
       parser->token.signCode == DIFFERENT ||
       parser->token.signCode == SMALLER_EQUAL ||
       parser->token.signCode == SMALLER_THAN ||
       parser->token.signCode == LARGER_EQUAL ||
       parser->token.signCode == LARGER_THAN)) {
    opRelFound = true;
  }

  // opRel is optional here
  if (opRelFound) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
    enum SYNTAX_ERROR error = exprSimp(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR exprSimp(struct Parser *parser) {
  // ignore plus or minus
  if (parser->token.category == SIGN &&
      (parser->token.signCode == PLUS || parser->token.signCode == MINUS)) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);
  }

  enum SYNTAX_ERROR error = termo(parser);
  if (error != NO_ERROR) {
    return error;
  }

  while (parser->token.category == SIGN &&
         (parser->token.signCode == PLUS || parser->token.signCode == MINUS ||
          parser->token.signCode == OR)) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);

    enum SYNTAX_ERROR error = termo(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR termo(struct Parser *parser) {
  enum SYNTAX_ERROR error = fator(parser);
  if (error != NO_ERROR) {
    return error;
  }

  while (parser->token.category == SIGN &&
         (parser->token.signCode == STAR || parser->token.signCode == SLASH ||
          parser->token.signCode == AND)) {
    parser->token = lexerGetNextChar(parser->fd, parser->lineCount);

    enum SYNTAX_ERROR error = fator(parser);
    if (error != NO_ERROR) {
      return error;
    }
  }

  return NO_ERROR;
}
