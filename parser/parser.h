#ifndef PARSER_H
#define PARSER_H

#include "../lexer/types.h"
#include "./syntax_error.h"
#include <stdio.h>

struct Parser {
  struct Token token;
  FILE* fd;
  int *lineCount;
};

enum SYNTAX_ERROR op_rel(struct Parser parser);

enum SYNTAX_ERROR fator(struct Parser parser);

enum SYNTAX_ERROR expr(struct Parser parser);

enum SYNTAX_ERROR arrayFator(struct Parser parser);

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
