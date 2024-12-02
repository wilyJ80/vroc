#include "syntax_error.h"
#include <stdbool.h>

void printSyntaxError(enum SYNTAX_ERROR error) {
  struct ErrorMessage messages[80] = {
      {NO_ERROR, "No syntax errors\n"},
      // decl_list_var
      {TYPE_NOT_DETECTED, "Type not detected\n"},
      {INVALID_TYPE, "Invalid type\n"},
      {INVALID_VAR_LIST_DEC, "Invalid variable list declaration\n"},
      // decl_var
      {NO_VAR_ID, "No variable ID\n"},
      {NO_VAR_ASSIGNMENT, "No variable assignment\n"},
      {INVALID_VAR_TYPE_INIT, "Invalid type in variable initialization\n"},
      {INVALID_ARRAY_SUBSCRIPT_DEC, "Invalid subscript in array declaration\n"},
      {INVALID_ARRAY_BRACKET_DEC_OPEN,
       "Invalid bracket opening in array declaration\n"},
      {INVALID_ARRAY_BRACKET_DEC_CLOSE,
       "Invalid bracket closing in array declaration\n"},
      {INVALID_ARRAY_INIT_CURLY_OPEN,
       "Invalid curly bracket opening in array initialization\n"},
      {INVALID_ARRAY_INIT_CURLY_CLOSE,
       "Invalid curly bracket closing in array initialization\n"},
      {INVALID_ARRAY_TYPE_INIT, "Invalid type in array initialization\n"},
      {INVALID_ARRAY_MULTIPLE_ITEM_INIT,
       "Invalid multiple item initialization in array\n"},
      // decl_def_proc
      {INVALID_FUNCTION_KEYWORD, "Invalid function keyword\n"},
      {NO_PROTO_ID, "No ID found for function prototype\n"},
      {INVALID_PROTO_PAREN_OPEN, "No opening paren in function prototype\n"},
      {INVALID_PROTO_PAREN_CLOSE, "No closing paren in function prototype\n"},
      {INVALID_PROTO_PARAM_TYPE,
       "Invalid parameter type for function prototype\n"},
      {INVALID_ARRAY_PROTO_PARAM_BRACKET_OPEN,
       "No opening bracket for function prototype array parameter\n"},
      {INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE,
       "No closing bracket for function prototype array parameter\n"},
      {INVALID_PROTO_PARAM_LIST,
       "Invalid multiple parameters in function prototype\n"},
      {NO_DEF_ID, "No valid ID for function definition\n"},
      {INVALID_DEF_PARAM_TYPE,
       "No valid parameter type for function definition\n"},
      {NO_DEF_PARAM_ID, "No ID detected for function definition parameter\n"},
      {INVALID_ARRAY_DEF_PARAM_SUBSCRIPT_TYPE,
       "No valid type for subscript in array parameter of function "
       "definition\n"},
      {INVALID_ARRAY_DEF_PARAM_BRACKET_OPEN,
       "No opening bracket detected for function definition array parameter\n"},
      {INVALID_ARRAY_DEF_PARAM_BRACKET_CLOSE,
       "No closing bracket detected for function definition array parameter\n"},
      {INVALID_DEF_PARAM_LIST,
       "Invalid parameter list for function definition\n"},
      {NO_DEF_END_KEYWORD,
       "End keyword for function definition not detected\n"},
      // cmd
      {INVALID_CMD_CONTENT, "Invalid command keyword or content\n"},
      // cmd (do)
      {INVALID_FUNCTION_CALL_ID, "No valid id detected for function call\n"},
      {INVALID_FUNCTION_CALL_PAREN_OPEN,
       "No paren open detected for function call\n"},
      {INVALID_FUNCTION_CALL_PAREN_CLOSE,
       "No paren close detected for function call\n"},
      {INVALID_FUNCTION_CALL_ARGUMENT_LIST,
       "Invalid argument list for function call\n"},
      // cmd (while)
      {INVALID_WHILE_PAREN_OPEN, "No paren open after while keyword\n"},
      {INVALID_WHILE_PAREN_CLOSE, "No paren close after while keyword\n"},
      {NO_WHILE_END_KEYWORD, "Unterminated while keyword\n"},
      // cmd (var)
      {NO_FOR_ID, "No valid id for var loop\n"},
      {NO_FOR_FROM, "No from keyword detected for var loop\n"},
      {NO_FOR_EXPR1, "No first expression detected for var loop\n"},
      {NO_FOR_TO_OR_DT, "No to or dt keyword detected for var loop\n"},
      {NO_FOR_EXPR2, "No second expression detected for var loop\n"},
      {INVALID_FOR_BY_INC_OR_DEC_TYPE,
       "Invalid type for increment/decrement in the var loop\n"},
      {NO_FOR_END_KEYWORD, "No end keyword for var loop\n"},
      // cmd (if)
      {INVALID_IF_PAREN_OPEN, "No paren opening in if conditional\n"},
      {INVALID_IF_PAREN_CLOSE, "No paren closing in if conditional\n"},
      {INVALID_ELIF_PAREN_OPEN, "No paren opening in elif conditional\n"},
      {INVALID_ELIF_PAREN_CLOSE, "No paren closing in elif conditional\n"},
      {NO_IF_END_KEYWORD, "No end keyword for if conditional\n"},
      // getint
      {NO_GETINT_ID, "No ID detected for getint\n"},
      // getreal
      {NO_GETREAL_ID, "NO ID detected for getreal\n"},
      // getchar
      {NO_GETCHAR_ID, "No ID detected for getchar\n"},
      // getstr
      {NO_GETSTR_ID, "No ID detected for getstr\n"},
      // putint
      {INVALID_PUTINT_ELEMENT, "Invalid type for putint element\n"},
      // putreal
      {INVALID_PUTREAL_ELEMENT, "Invalid type for putreal element\n"},
      // putchar
      {INVALID_PUTCHAR_ELEMENT, "Invalid type for putchar element\n"},
      // putstr
      {INVALID_PUTSTR_ELEMENT, "Invalid type for putstr element\n"},
      // atrib
      {NO_ATRIB_ID, "No ID detected for expression assign\n"},
      {NO_ATRIB_ASSIGN, "No assign symbol for expression assign\n"},
      {NO_ATRIB_EXPR, "No expression assigned to expression assign\n"},
      // expr
      {NO_EXPR_EXPR_SIMP, "No simple expression for expression\n"},
      {NO_EXPR_EXPR_SIMP_AFTER_OP_REL,
       "No simple expression after operation\n"},
      // expr_simp
      {NO_EXPR_SIMP_TERM, "No term detected in simple expression\n"},
      {NO_EXPR_SIMP_TERM_VALID_SIGN_BEFORE,
       "No valid sign detected before simple expression\n"},
      {NO_EXPR_SIMP_TERM_VALID_SIGN_AFTER,
       "No valid sign detected after simple expression\n"},
      {NO_EXPR_SIMP_TERM_AFTER_VALID_SIGN,
       "No valid term after valid sign in simple expression\n"},
      // termo
      {NO_TERM_FACTOR, "No factor detected in term\n"},
      {NO_TERM_FACTOR_VALID_SIGN_AFTER, "Invalid sign after term\n"},
      {NO_TERM_FACTOR_AFTER_FACTOR_VALID_SIGN,
       "No factor detected after valid sign in term\n"},
      // fator
      {NO_FACTOR_VALID_START_SYMBOL, "No factor valid start symbol\n"},
      {INVALID_FACTOR_ARRAY_BRACKET_OPEN, "No factor array bracket opening\n"},
      {INVALID_FACTOR_ARRAY_BRACKET_CLOSE, "No factor array bracket closing\n"},
      {INVALID_FACTOR_EXPR_PAREN_OPEN, "No factor expression paren opening\n"},
      {INVALID_FACTOR_EXPR_PAREN_CLOSE, "No factor expression paren closing\n"},
      {NO_FACTOR_AFTER_BANG, "No factor detected after bang in factor\n"},
      // op_rel
      {INVALID_OPERATOR, "Invalid operator in expression\n"}
  };
  // TODO: Finish function
}
