#ifndef PARSER_INTEGRATION_TESTS_H
#define PARSER_INTEGRATION_TESTS_H

#include "../parser/parser.h"
#include "../parser/syntax_error.h"
#include "../lexer/lexer.h"
#include "../lexer/types.h"

enum SYNTAX_ERROR setupError(const char* mockData);

void progStartKeyword();

void declListVarInvalidType();
void declVarNoId();
void declListVarMulti();
void declListVarMultiFail();

void declVarArrayInvalidSubscript();
void declVarArrayDidntClose();
void declVarBadInit();
void declVarArrayBadInitCurly();
void declVarArrayMultiTooMany();
void declVarArrayInvalidTypeInit();
void declVarArrayMultiInit();
void declVarArrayBadClose();

void declDefProcNoId();
void declDefProcProtNoOpenParen();
void declDefProcProtoInvalidParamType();
void declDefProcProtoNoParamId();
void declDefProcProtoNoValidTokenAfterType();
void declDefProcProtoUnclosedArrayParam();
void declDefProcProtoInvalid2dArrayOpen();
void declDefProcProtoInvalid2dArrayClose();
void declDefProcProtoInvalid3dArray();
void declDefProcProtoMultiParams();
void declDefProcProtoNoParenClose();
void declDefProcProtoTwoProts();

void declDefProcDefDefWorksToo();
void declDefProcDefNoParenOpen();
void declDefProcDefBadParamType();
void declDefProcDefNoParamId();
void declDefProcDefNoValidTokenAfterId();

#endif // !PARSER_INTEGRATION_TESTS_H
