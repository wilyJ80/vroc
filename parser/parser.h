#ifndef PARSER_H
#define PARSER_H

#include "../lexer/types.h"
#include "./syntax_error.h"
#include <stdio.h>

enum SYNTAX_ERROR op_rel(FILE *fd, int *lineCount);

/*void prog(FILE *fd, int *lineCount);*/
/**/
/*void declListVar(FILE *fd, int *lineCount, struct Token token);*/
/*void declDefProc(FILE *fd, int *lineCount, struct Token token);*/
/**/
/*void tipo(FILE *fd, int *lineCount, struct Token token);*/
/*void declVar(FILE *fd, int *lineCount, struct Token token);*/
/**/
/*void arrayDeclaration(FILE *fd, int *lineCount, struct Token token);*/
/*void arrayInitialization(FILE *fd, int *lineCount, struct Token token);*/

#endif
