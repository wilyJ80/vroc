#ifndef PARSER_TABLE_UTIL_H
#define PARSER_TABLE_UTIL_H

#include "./parser.h"
#include "./symbol_table.h"

void addTableRow(struct Parser *parser, struct Row row);

void clearRow(struct Row *row);

void fillArrayType(struct Row *row, int dimensions);

void arrayType(struct Row *row, int dimensions);

#endif
