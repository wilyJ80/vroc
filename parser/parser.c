#include "parser.h"
#include "../lexer/transition.h"
#include "parser_table_util.h"
#include "symbol_table.h"
#include "syntax_error.h"
#include "token_cmp.h"
#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_DIMENSIONS 2

struct Row row;

/**
 * prog accepts repetitions of declarations of variables (decl_list_var), or
 * procedures (decl_list_proc).
 */
enum SYNTAX_ERROR prog(struct Parser *parser) {
  // Both declaration of variables and procedures start with reserved words.
  // Valid variable declaration start tokens
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         (tokenSignCodeMatchAny(parser, 5, CONST, CHAR, INT, REAL, BOOL))) {
    row.scope = GBL;
    row.category = VG;
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
  row.passage = PASS_NA;
  row.zombie = ZOMBIE_NA;
  row.array = VS;
  row.dim1 = 0;
  row.dim2 = 0;
  row.isConst = false;
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, CONST)) {
    row.isConst = true;
    consumeTokenFrom(parser);
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 4, INT, REAL, CHAR, BOOL))) {
    return INVALID_TYPE;
  }

  switch (parser->token.signCode) {
  case INT:
    row.type = TYPE_INT;
    break;
  case REAL:
    row.type = TYPE_REAL;
    break;
  case CHAR:
    row.type = TYPE_CHAR;
    break;
  case BOOL:
    row.type = TYPE_BOOL;
    break;
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
  strcpy(row.lexeme, parser->token.lexeme);
  consumeTokenFrom(parser);

  // simple variable assignment
  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 1, ASSIGN)) {
    consumeTokenFrom(parser);

    if (!(tokenCategoryMatchAny(parser, 3, INTCON, REALCON, CHARCON))) {
      return INVALID_VAR_TYPE_INIT;
    }

    if (tokenCategoryMatchAny(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, CONST)) {
      switch (parser->token.signCode) {
      case INTCON:
        row.constValue.intValue = parser->token.intValue;
        break;
      case REALCON:
        row.constValue.doubleValue = parser->token.intValue;
        break;
      case CHARCON:
        // does it work?
        row.constValue.charValue = parser->token.lexeme[0];
        break;
      }
    }

    consumeTokenFrom(parser);
    return NO_ERROR;
  }

  // the following is related to array variable: single variable returned early
  // above
  int dimensions = 0;
  int arr[2];
  while (tokenCategoryMatchAll(parser, 1, SIGN) &&
         tokenSignCodeMatchAny(parser, 1, OPEN_BRACK)) {
    dimensions++;
    consumeTokenFrom(parser);

    if (!(tokenCategoryMatchAll(parser, 2, INTCON, ID))) {
      return INVALID_ARRAY_SUBSCRIPT_DEC;
    }

    if (tokenCategoryMatchAll(parser, 1, INTCON)) {
      if (dimensions == 1) {
        row.dim1 = parser->token.intValue;
      } else if (dimensions == 2) {
        row.dim2 = parser->token.intValue;
      } else {
        // TODO:
      }
    } else if (tokenCategoryMatchAll(parser, 1, ID)) {
      if (dimensions == 1) {
        // TODO:
        row.dim1 = 1;
      } else if (dimensions == 2) {
        row.dim2 = 1;
      }
    }

    consumeTokenFrom(parser);
    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, CLOSE_BRACK))) {
      return INVALID_ARRAY_BRACKET_DEC_CLOSE;
    }
    consumeTokenFrom(parser);

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

    switch (dimensions) {
    case 0:
      row.array = VS;
      break;
    case 1:
      row.array = VET;
      break;
    case 2:
      row.array = MAT;
      break;
    }

    addTableRow(parser, row);
    consumeTokenFrom(parser);
    return NO_ERROR;
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR declVarArrayInit(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 3, INTCON, REALCON, CHARCON))) {
    return INVALID_ARRAY_TYPE_INIT;
  }

  if (tokenCategoryMatchAny(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, CONST)) {
    switch (parser->token.signCode) {
    case INTCON:
      row.constValue.intValue = parser->token.intValue;
      row.constValue.type = TYPE_INT;
      break;
    case REALCON:
      row.constValue.doubleValue = parser->token.intValue;
      row.constValue.type = TYPE_REAL;
      break;
    case CHARCON:
      // does it work?
      row.constValue.charValue = parser->token.lexeme[0];
      row.constValue.type = TYPE_CHAR;
      break;
    }
  } else {
    row.constValue.type = TYPE_NA;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR declDefProc(struct Parser *parser) {
  row.array = ARRAY_NA;
  row.scope = GBL;
  row.isConst = false;
  row.zombie = ZOMBIE_NA;
  row.passage = PASS_NA;
  row.type = TYPE_NA;
  row.category = PRT;

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
  addTableRow(parser, row);
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
    }

    if (paramMandatory &&
        !(tokenCategoryMatchAll(parser, 1, RSV) &&
          tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL))) {
      return NO_PROTO_TYPE_AFTER_REF;
    }

    if (tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL)) {
      consumeTokenFrom(parser);

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

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_PROTO_PAREN_CLOSE;
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

  // is declListVar
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 5, CONST, CHAR, INT, REAL, BOOL)) {
    enum SYNTAX_ERROR error = declListVar(parser);
    if (error) {
      return error;
    }
  }

  // is cmd
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                               GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL,
                               PUTCHAR, PUTSTR, ID)) {
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
  bool paramMandatory = false;
  bool isArray = false;
  bool isEmpty = true;
  // while it's a param, delimited by comma
  do {
    // consume comma from subsequent iterations
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, COMMA)) {
      consumeTokenFrom(parser);
    }

    // & is optional
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, REF)) {
      paramMandatory = true;
      consumeTokenFrom(parser);
    }

    if (paramMandatory &&
        !(tokenCategoryMatchAll(parser, 1, RSV) &&
          tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL))) {
      return NO_DEF_TYPE_AFTER_REF;
    }

    if (tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 4, CHAR, INT, REAL, BOOL)) {
      isEmpty = false;
      consumeTokenFrom(parser);

      while (tokenCategoryMatchAll(parser, 1, SIGN) &&
             tokenSignCodeMatchAny(parser, 1, OPEN_BRACK)) {
        isArray = true;
        consumeTokenFrom(parser);

        if (!(tokenCategoryMatchAll(parser, 2, ID, INTCON))) {
          return INVALID_ARRAY_DEF_PARAM_SUBSCRIPT_TYPE;
        }
        consumeTokenFrom(parser);

        if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
              tokenSignCodeMatchAny(parser, 1, CLOSE_BRACK))) {
          return INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE;
        }
        consumeTokenFrom(parser);
      }
    }

    if (!isArray && !isEmpty) {
      consumeTokenFrom(parser);
    }

  } while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, COMMA));

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_DEF_PAREN_CLOSE;
  }

  // can be endp, declListVar, or cmd
  consumeTokenFrom(parser);

  return NO_ERROR;
}

enum SYNTAX_ERROR cmd(struct Parser *parser) {
  if (tokenCategoryMatchAll(parser, 1, ID)) {
    parser->token.signCode = ID;
  }
  enum RESERVED sign = parser->token.signCode;
  consumeTokenFrom(parser);

  enum SYNTAX_ERROR error;
  switch (sign) {
  case GETINT:
    if (!(tokenCategoryMatchAny(parser, 1, ID))) {
      return NO_GETINT_ID;
    }
    consumeTokenFrom(parser);
    break;
  case GETREAL:
    if (!(tokenCategoryMatchAny(parser, 1, ID))) {
      return NO_GETREAL_ID;
    }
    consumeTokenFrom(parser);
    break;
  case GETCHAR:
    if (!(tokenCategoryMatchAny(parser, 1, ID))) {
      return NO_GETCHAR_ID;
    }
    consumeTokenFrom(parser);
    break;
  case GETSTR:
    if (!(tokenCategoryMatchAny(parser, 1, ID))) {
      return NO_GETSTR_ID;
    }
    consumeTokenFrom(parser);
    break;
  case PUTINT:
    if (!(tokenCategoryMatchAny(parser, 2, ID, INTCON))) {
      return INVALID_PUTINT_ELEMENT;
    }
    consumeTokenFrom(parser);
    break;
  case PUTREAL:
    if (!(tokenCategoryMatchAny(parser, 2, ID, REALCON))) {
      return INVALID_PUTREAL_ELEMENT;
    }
    consumeTokenFrom(parser);
    break;
  case PUTCHAR:
    if (!(tokenCategoryMatchAny(parser, 2, ID, CHARCON))) {
      return INVALID_PUTCHAR_ELEMENT;
    }
    consumeTokenFrom(parser);
    break;
  case PUTSTR:
    if (!(tokenCategoryMatchAny(parser, 2, ID, STRINGCON))) {
      return INVALID_PUTSTR_ELEMENT;
    }
    consumeTokenFrom(parser);
    break;
  case DO:
    error = cmdDo(parser);
    if (error != NO_ERROR) {
      return error;
    }
    break;
  case WHILE:
    error = cmdWhile(parser);
    if (error != NO_ERROR) {
      return error;
    }
    break;
  case VAR:
    error = cmdVar(parser);
    if (error != NO_ERROR) {
      return error;
    }
    break;
  case IF:
    error = cmdIf(parser);
    if (error != NO_ERROR) {
      return error;
    }
    break;
  case ID:
    error = cmdAtrib(parser);
    if (error != NO_ERROR) {
      return error;
    }
    break;
  case GETOUT:
    consumeTokenFrom(parser);
    break;
  default:
    break;
  }

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
  if (tokenCategoryMatchAll(parser, 1, ID)) {
    consumeTokenFrom(parser);

    while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, OPEN_PAR)) {
      consumeTokenFrom(parser);
      enum SYNTAX_ERROR error = expr(parser);
      if (error) {
        return error;
      }

      if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
            tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
        return INVALID_FACTOR_ARRAY_BRACKET_CLOSE;
      }
    }

    return NO_ERROR;

  } else if (tokenCategoryMatchAll(parser, 1, INTCON)) {
    consumeTokenFrom(parser);
    return NO_ERROR;

  } else if (tokenCategoryMatchAll(parser, 1, REALCON)) {
    consumeTokenFrom(parser);
    return NO_ERROR;

  } else if (tokenCategoryMatchAll(parser, 1, CHARCON)) {
    consumeTokenFrom(parser);
    return NO_ERROR;

  } else if (tokenCategoryMatchAll(parser, 1, OPEN_PAR)) {
    consumeTokenFrom(parser);

    enum SYNTAX_ERROR error = expr(parser);
    if (error) {
      return error;
    }

    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
      return INVALID_FACTOR_EXPR_PAREN_CLOSE;
    }

    consumeTokenFrom(parser);
    return NO_ERROR;

  } else if (tokenCategoryMatchAll(parser, 1, SIGN) &&
             tokenSignCodeMatchAny(parser, 1, NEGATION)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = fator(parser);
    if (error) {
      return error;
    }

    return NO_ERROR;
  }

  return NO_FACTOR_VALID_START_SYMBOL;
}

enum SYNTAX_ERROR cmdIf(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, OPEN_PAR))) {
    return INVALID_IF_PAREN_OPEN;
  }

  consumeTokenFrom(parser);
  enum SYNTAX_ERROR error = expr(parser);
  if (error) {
    return error;
  }

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_IF_PAREN_CLOSE;
  }
  consumeTokenFrom(parser);

  while (tokenCategoryMatchAll(parser, 1, ID) ||
         (tokenCategoryMatchAll(parser, 1, RSV) &&
          tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                                GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL,
                                PUTCHAR, PUTSTR))) {
    enum SYNTAX_ERROR error = cmd(parser);
    if (error) {
      return error;
    }
  }

  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 1, ELIF)) {
    consumeTokenFrom(parser);

    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, OPEN_PAR))) {
      return INVALID_ELIF_PAREN_OPEN;
    }

    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = expr(parser);
    if (error) {
      return error;
    }
    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
      return INVALID_ELIF_PAREN_CLOSE;
    }
    while (tokenCategoryMatchAll(parser, 1, RSV) &&
           tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                                 GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL,
                                 PUTCHAR, PUTSTR, ID)) {
      enum SYNTAX_ERROR error = cmd(parser);
      if (error) {
        return error;
      }
    }
  }

  // else is optional
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, ELSE)) {
    consumeTokenFrom(parser);
    while (tokenCategoryMatchAll(parser, 1, ID) ||
           (tokenCategoryMatchAll(parser, 1, RSV) &&
            tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT,
                                  GETINT, GETREAL, GETCHAR, GETSTR, PUTINT,
                                  PUTREAL, PUTCHAR, PUTSTR, ID))) {
      enum SYNTAX_ERROR error = cmd(parser);
      if (error) {
        return error;
      }
    }
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, ENDI))) {
    return NO_IF_END_KEYWORD;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR cmdVar(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, ID))) {
    return NO_FOR_ID;
  }

  consumeTokenFrom(parser);
  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, FROM))) {
    return NO_FOR_FROM;
  }

  consumeTokenFrom(parser);
  enum SYNTAX_ERROR error = expr(parser);
  if (error) {
    return error;
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 2, TO, DT))) {
    return NO_FOR_TO_OR_DT;
  }

  consumeTokenFrom(parser);
  enum SYNTAX_ERROR error2 = expr(parser);
  if (error2 != NO_ERROR) {
    return error2;
  }

  // optional by
  if (tokenCategoryMatchAll(parser, 1, RSV) &&
      tokenSignCodeMatchAny(parser, 1, BY)) {
    consumeTokenFrom(parser);
    if (!(tokenCategoryMatchAny(parser, 2, INTCON, ID))) {
      return INVALID_FOR_BY_INC_OR_DEC_TYPE;
    }
    consumeTokenFrom(parser);
  }

  // suggested change: do while token is valid cmd starter
  while (tokenCategoryMatchAll(parser, 1, RSV) &&
         tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                               GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL,
                               PUTCHAR, PUTSTR, ID)) {
    enum SYNTAX_ERROR error = cmd(parser);
    if (error) {
      return error;
    }
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, ENDV))) {
    return NO_FOR_END_KEYWORD;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR cmdWhile(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, OPEN_PAR))) {
    return INVALID_WHILE_PAREN_OPEN;
  }

  consumeTokenFrom(parser);
  enum SYNTAX_ERROR error = expr(parser);
  if (error) {
    return error;
  }

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_WHILE_PAREN_CLOSE;
  }

  consumeTokenFrom(parser);
  while (tokenCategoryMatchAll(parser, 1, ID) ||
         (tokenCategoryMatchAll(parser, 1, RSV) &&
          tokenSignCodeMatchAny(parser, 14, DO, WHILE, VAR, IF, GETOUT, GETINT,
                                GETREAL, GETCHAR, GETSTR, PUTINT, PUTREAL,
                                PUTCHAR, PUTSTR, ID))) {
    enum SYNTAX_ERROR error2 = cmd(parser);
    if (error2) {
      return error2;
    }
  }

  if (!(tokenCategoryMatchAll(parser, 1, RSV) &&
        tokenSignCodeMatchAny(parser, 1, ENDW))) {
    return NO_WHILE_END_KEYWORD;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR cmdAtrib(struct Parser *parser) {
  while (tokenCategoryMatchAll(parser, 1, SIGN) &&
         tokenSignCodeMatchAny(parser, 1, OPEN_BRACK)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = expr(parser);
    if (error) {
      return error;
    }

    if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
          tokenSignCodeMatchAny(parser, 1, CLOSE_BRACK))) {
      return NO_ATRIB_BRACKET_CLOSE;
    }
    consumeTokenFrom(parser);
  }

  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, ASSIGN))) {
    return NO_ATRIB_ASSIGN;
  }
  consumeTokenFrom(parser);

  enum SYNTAX_ERROR error = expr(parser);
  if (error) {
    return error;
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR cmdDo(struct Parser *parser) {
  if (!(tokenCategoryMatchAll(parser, 1, ID))) {
    return INVALID_FUNCTION_CALL_ID;
  }

  // is id, ok... then it should open a paren
  consumeTokenFrom(parser);
  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, OPEN_PAR))) {
    return INVALID_FUNCTION_CALL_PAREN_OPEN;
  }

  consumeTokenFrom(parser);
  do {
    // consume if comma from previous iteration
    if (tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, COMMA)) {
      consumeTokenFrom(parser);
    }
    enum SYNTAX_ERROR error = expr(parser);
    if (error) {
      return error;
    }
  } while (tokenCategoryMatchAll(parser, 1, SIGN) &&
           tokenSignCodeMatchAny(parser, 1, COMMA));

  // check paren close
  if (!(tokenCategoryMatchAll(parser, 1, SIGN) &&
        tokenSignCodeMatchAny(parser, 1, CLOSE_PAR))) {
    return INVALID_FUNCTION_CALL_PAREN_CLOSE;
  }

  consumeTokenFrom(parser);
  return NO_ERROR;
}

enum SYNTAX_ERROR expr(struct Parser *parser) {
  enum SYNTAX_ERROR error = exprSimp(parser);
  if (error) {
    return error;
  }

  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 6, COMPARISON, DIFFERENT, SMALLER_EQUAL,
                            SMALLER_THAN, LARGER_EQUAL, LARGER_THAN)) {
    consumeTokenFrom(parser);
    enum SYNTAX_ERROR error = exprSimp(parser);
    if (error) {
      return error;
    }
  }

  return NO_ERROR;
}

enum SYNTAX_ERROR exprSimp(struct Parser *parser) {
  if (tokenCategoryMatchAll(parser, 1, SIGN) &&
      tokenSignCodeMatchAny(parser, 2, PLUS, MINUS)) {
    consumeTokenFrom(parser);
  }

  enum SYNTAX_ERROR error = termo(parser);
  if (error) {
    return error;
  }

  while (tokenCategoryMatchAll(parser, 1, SIGN) &&
         tokenSignCodeMatchAny(parser, 3, PLUS, MINUS, OR)) {
    consumeTokenFrom(parser);

    enum SYNTAX_ERROR error = termo(parser);
    if (error) {
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

  while (tokenCategoryMatchAll(parser, 1, SIGN) &&
         tokenSignCodeMatchAny(parser, 3, STAR, SLASH, AND)) {
    consumeTokenFrom(parser);

    enum SYNTAX_ERROR error = fator(parser);
    if (error) {
      return error;
    }
  }

  return NO_ERROR;
}
