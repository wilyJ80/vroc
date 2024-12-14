#include "syntax_error.h"
#include <stdbool.h>
#include <stdio.h>

#define ERROR_QTY 92

// ANSI escape codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

void printSyntaxError(enum SYNTAX_ERROR error, int *lineCount) {
  struct ErrorMessage messages[ERROR_QTY] = {
      {NO_ERROR, "No syntax errors"},
      // prog
      {INVALID_PROG_START_KEYWORD, "Invalid program start keyword"},
      // decl_list_var
      {TYPE_NOT_DETECTED, "Type not detected"},
      {INVALID_TYPE, "Invalid type"},
      {INVALID_VAR_LIST_DEC, "Invalid variable list declaration"},
      // decl_var
      {NO_VAR_ID, "No variable ID"},
      {NO_VAR_ASSIGNMENT, "No variable assignment"},
      {INVALID_VAR_TYPE_INIT, "Invalid type in variable initialization"},
      {INVALID_ARRAY_SUBSCRIPT_DEC, "Invalid subscript in array declaration"},
      {INVALID_ARRAY_BRACKET_DEC_OPEN,
       "Invalid bracket opening in array declaration"},
      {INVALID_ARRAY_BRACKET_DEC_CLOSE,
       "Invalid bracket closing in array declaration"},
      {INVALID_ARRAY_INIT_CURLY_OPEN,
       "Invalid curly bracket opening in array initialization"},
      {INVALID_ARRAY_INIT_CURLY_CLOSE,
       "Invalid curly bracket closing in array initialization"},
      {INVALID_ARRAY_TYPE_INIT, "Invalid type in array initialization"},
      {INVALID_ARRAY_DIMENSION_DECLARATION,
       "Invalid dimension declaration in array"},
      {INVALID_ARRAY_MULTIPLE_ITEM_INIT,
       "Invalid multiple item initialization in array"},
      {INVALID_ARRAY_END,
       "Invalid array declaration end: expected comma or bracket closing"},
      // decl_def_proc
      {INVALID_FUNCTION_KEYWORD, "Invalid function keyword"},
      {NO_FUNCTION_ID, "No valid id for function"},
      {NO_PROTO_ID, "No ID found for function prototype"},
      {INVALID_PROTO_PAREN_OPEN, "No opening paren in function prototype"},
      {INVALID_PROTO_PAREN_CLOSE, "No closing paren in function prototype"},
      {NO_PROTO_TYPE_AFTER_REF,
       "No prototype paramater type found after & token"},
      {INVALID_PROTO_PARAM_TYPE,
       "Invalid parameter type for function prototype"},
      {NO_PROTO_VALID_TOKEN_AFTER_TYPE, "No valid token found after function "
                                        "prototype type, expected bracket open "
                                        "or comma"},
      {INVALID_ARRAY_PROTO_PARAM_BRACKET_OPEN,
       "No opening bracket for function prototype array parameter"},
      {INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE,
       "No closing bracket for function prototype array parameter"},
      {INVALID_PROTO_PARAM_LIST,
       "Invalid multiple parameters in function prototype"},
      {NO_PROTO_VALID_TOKEN_AFTER_BRACKET_CLOSE,
       "Expected valid token after bracket closing: bracket opening, comma or "
       "paren close"},
      {NO_DEF_ID, "No valid ID for function definition"},
      {INVALID_DEF_PAREN_OPEN, "No valid paren open for function definition"},
      {INVALID_DEF_PAREN_CLOSE, "No valid paren close for function definition"},
      {INVALID_DEF_PARAM_TYPE,
       "No valid parameter type for function definition"},
      {NO_DEF_VALID_TOKEN_AFTER_ID, "No valid token after def id, expected "
                                    "open bracket, comma or paren close"},
      {NO_DEF_PARAM_ID, "No ID detected for function definition parameter"},
      {INVALID_ARRAY_DEF_PARAM_SUBSCRIPT_TYPE,
       "No valid type for subscript in array parameter of function "
       "definition"},
      {INVALID_ARRAY_DEF_PARAM_BRACKET_OPEN,
       "No opening bracket detected for function definition array parameter"},
      {INVALID_ARRAY_DEF_PARAM_BRACKET_CLOSE,
       "No closing bracket detected for function definition array parameter"},
      {INVALID_DEF_PARAM_LIST,
       "Invalid parameter list for function definition"},
      {NO_DEF_END_KEYWORD, "End keyword for function definition not detected"},
      {NO_DEF_VALID_TOKEN_AFTER_PAREN,
       "No valid token after def paren close, expected endp, variable "
       "declaration, or cmd"},
      {NO_FUNCTION_END_PAREN_CLOSE, "No paren close for function end"},
      // cmd
      {INVALID_CMD_CONTENT, "Invalid command keyword or content"},
      // cmd (do)
      {INVALID_FUNCTION_CALL_ID, "No valid id detected for function call"},
      {INVALID_FUNCTION_CALL_PAREN_OPEN,
       "No paren open detected for function call"},
      {INVALID_FUNCTION_CALL_PAREN_CLOSE,
       "No paren close detected for function call"},
      {INVALID_FUNCTION_CALL_ARGUMENT_LIST,
       "Invalid argument list for function call"},
      // cmd (while)
      {INVALID_WHILE_PAREN_OPEN, "No paren open after while keyword"},
      {INVALID_WHILE_PAREN_CLOSE, "No paren close after while keyword"},
      {NO_WHILE_END_KEYWORD, "Unterminated while keyword"},
      // cmd (var)
      {NO_FOR_ID, "No valid id for var loop"},
      {NO_FOR_FROM, "No from keyword detected for var loop"},
      {NO_FOR_EXPR1, "No first expression detected for var loop"},
      {NO_FOR_TO_OR_DT, "No to or dt keyword detected for var loop"},
      {NO_FOR_EXPR2, "No second expression detected for var loop"},
      {INVALID_FOR_BY_INC_OR_DEC_TYPE,
       "Invalid type for increment/decrement in the var loop"},
      {NO_FOR_END_KEYWORD, "No end keyword for var loop"},
      // cmd (if)
      {INVALID_IF_PAREN_OPEN, "No paren opening in if conditional"},
      {INVALID_IF_PAREN_CLOSE, "No paren closing in if conditional"},
      {INVALID_ELIF_PAREN_OPEN, "No paren opening in elif conditional"},
      {INVALID_ELIF_PAREN_CLOSE, "No paren closing in elif conditional"},
      {NO_IF_END_KEYWORD, "No end keyword for if conditional"},
      // getint
      {NO_GETINT_ID, "No ID detected for getint"},
      // getreal
      {NO_GETREAL_ID, "NO ID detected for getreal"},
      // getchar
      {NO_GETCHAR_ID, "No ID detected for getchar"},
      // getstr
      {NO_GETSTR_ID, "No ID detected for getstr"},
      // putint
      {INVALID_PUTINT_ELEMENT, "Invalid type for putint element"},
      // putreal
      {INVALID_PUTREAL_ELEMENT, "Invalid type for putreal element"},
      // putchar
      {INVALID_PUTCHAR_ELEMENT, "Invalid type for putchar element"},
      // putstr
      {INVALID_PUTSTR_ELEMENT, "Invalid type for putstr element"},
      // atrib
      {NO_ATRIB_ID, "No ID detected for expression assign"},
      {NO_ATRIB_ASSIGN, "No assign symbol for expression assign"},
      {NO_ATRIB_EXPR, "No expression assigned to expression assign"},
      {NO_ATRIB_VALID_TOKEN_AFTER_ID,
       "No valid token after identifier for assignment"},
      {NO_ATRIB_BRACKET_CLOSE, "Expected bracket closing at array assignment"},
      // expr
      {NO_EXPR_EXPR_SIMP, "No simple expression for expression"},
      {NO_EXPR_EXPR_SIMP_AFTER_OP_REL, "No simple expression after operation"},
      // expr_simp
      {NO_EXPR_SIMP_TERM, "No term detected in simple expression"},
      {NO_EXPR_SIMP_TERM_VALID_SIGN_BEFORE,
       "No valid sign detected before simple expression"},
      {NO_EXPR_SIMP_TERM_VALID_SIGN_AFTER,
       "No valid sign detected after simple expression"},
      {NO_EXPR_SIMP_TERM_AFTER_VALID_SIGN,
       "No valid term after valid sign in simple expression"},
      // termo
      {NO_TERM_FACTOR, "No factor detected in term"},
      {NO_TERM_FACTOR_VALID_SIGN_AFTER, "Invalid sign after term"},
      {NO_TERM_FACTOR_AFTER_FACTOR_VALID_SIGN,
       "No factor detected after valid sign in term"},
      // fator
      {NO_FACTOR_VALID_START_SYMBOL, "No factor valid start symbol"},
      {NO_FACTOR_VALID_SYMBOL_AFTER_ID, "No factor valid symbol after id"},
      {INVALID_FACTOR_ARRAY_BRACKET_OPEN, "No factor array bracket opening"},
      {INVALID_FACTOR_ARRAY_BRACKET_CLOSE, "No factor array bracket closing"},
      {INVALID_FACTOR_EXPR_PAREN_OPEN, "No factor expression paren opening"},
      {INVALID_FACTOR_EXPR_PAREN_CLOSE, "No factor expression paren closing"},
      {NO_FACTOR_AFTER_BANG, "No factor detected after bang in factor"},
      // op_rel
      {INVALID_OPERATOR, "Invalid operator in expression"}};

  for (int i = 0; i < ERROR_QTY; i++) {
    if (error == messages[i].error) {
      fprintf(stderr, RED "Syntax error: %s on line %d\n" RESET,
              messages[i].message, *lineCount);
    }
  }
}
