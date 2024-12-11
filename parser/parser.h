#ifndef PARSER_H
#define PARSER_H

#include "../lexer/types.h"
#include "./syntax_error.h"
#include <stdio.h>

struct Parser {
  struct Token token;
  FILE *fd;
  int *lineCount;
};

enum SYNTAX_ERROR op_rel(struct Parser *parser);

enum SYNTAX_ERROR fator(struct Parser *parser);

enum SYNTAX_ERROR expr(struct Parser *parser);

enum SYNTAX_ERROR arrayFator(struct Parser *parser);

enum SYNTAX_ERROR prog(struct Parser *parser);

enum SYNTAX_ERROR declListVar(struct Parser *parser);
enum SYNTAX_ERROR declDefProc(struct Parser *parser);

/*void tipo(FILE *fd, int *lineCount, struct Token token);*/
enum SYNTAX_ERROR declVar(struct Parser *parser);

enum SYNTAX_ERROR declProt(struct Parser *parser);

enum SYNTAX_ERROR declProtParam(struct Parser *parser);
/**/
/*void arrayDeclaration(FILE *fd, int *lineCount, struct Token token);*/
/*void arrayInitialization(FILE *fd, int *lineCount, struct Token token);*/

enum SYNTAX_ERROR declDef(struct Parser *parser);
enum SYNTAX_ERROR declDefParam(struct Parser *parser);
enum SYNTAX_ERROR declDefParamArray(struct Parser *parser);

enum SYNTAX_ERROR cmd(struct Parser *parser);
enum SYNTAX_ERROR getint(struct Parser *parser);

#endif
