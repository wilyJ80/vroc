#ifndef NEW_PARSER_TESTS_H
#define NEW_PARSER_TESTS_H

#include "../parser/syntax_error.h"
#include "../lexer/types.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

enum SYNTAX_ERROR newSetupError(const char *mockData);

void declarationsTest();

void protTest();

void defTest();

void defTestWithRepeatedVariableList();

void defTestWithFailedCmd();

void defTestDoesNotGoBackToDeclListVar();

#endif
