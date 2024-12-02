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
    {INVALID_ARRAY_BRACKET_DEC_OPEN, "Invalid bracket opening in array declaration\n"},
    {INVALID_ARRAY_BRACKET_DEC_CLOSE, "Invalid bracket closing in array declaration\n"},
    {INVALID_ARRAY_INIT_CURLY_OPEN, "Invalid curly bracket opening in array initialization\n"},
    {INVALID_ARRAY_INIT_CURLY_CLOSE, "Invalid curly bracket closing in array initialization\n"},
    {INVALID_ARRAY_TYPE_INIT, "Invalid type in array initialization\n"},
    {INVALID_ARRAY_MULTIPLE_ITEM_INIT, "Invalid multiple item initialization in array\n"},
    // decl_def_proc
    {INVALID_FUNCTION_KEYWORD, "Invalid function keyword\n"},
    {NO_PROTO_ID, "No ID found for function prototype\n"},
    {INVALID_PROTO_PAREN_OPEN, "No opening paren in function prototype\n"},
    {INVALID_PROTO_PAREN_CLOSE, "No closing paren in function prototype\n"},
    {INVALID_PROTO_PARAM_TYPE, "Invalid parameter type for function prototype\n"},
    {INVALID_ARRAY_PROTO_PARAM_BRACKET_OPEN, "No opening bracket for function prototype array parameter\n"},
    {INVALID_ARRAY_PROTO_PARAM_BRACKET_CLOSE, "No closing bracket for function prototype array parameter\n"},
    {INVALID_PROTO_PARAM_LIST, "Invalid multiple parameters in function prototype\n"},
    {NO_DEF_ID, "No valid ID for function definition\n"},


  };
}
