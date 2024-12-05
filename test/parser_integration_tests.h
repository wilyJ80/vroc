#ifndef PARSER_INTEGRATION_TESTS_H
#define PARSER_INTEGRATION_TESTS_H

#include "../parser/parser.h"
#include "../parser/syntax_error.h"
#include "../lexer/lexer.h"
#include "../lexer/types.h"

void progStartKeyword();
void declListVarInvalidType();
void declVarNoId();
void declListVarMulti();
void declVarArrayInvalidSubscript();
void declVarArrayDidntClose();
void declVarBadInit();

#endif // !PARSER_INTEGRATION_TESTS_H
