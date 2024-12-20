#ifndef NEW_PARSER_TESTS_H
#define NEW_PARSER_TESTS_H

#include "../parser/syntax_error.h"

enum SYNTAX_ERROR newSetupError(const char *mockData);

void declarationsTest();

void protTest();

#endif
