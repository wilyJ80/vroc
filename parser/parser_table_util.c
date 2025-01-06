#include "./parser_table_util.h"
#include <string.h>

void addTableRow(struct Parser *parser, struct Row row) {
  parser->symbolTable.top++;
  int index = parser->symbolTable.top;
  parser->symbolTable.row[index] = row;
}

void clearRow(struct Row *row) {
  memset(row, 0, sizeof(struct Row));
  memset(row->lexeme, 0, sizeof(row->lexeme));
}

void fillArrayType(struct Row *row, int dimensions) {
}

void arrayType(struct Row *row, int dimensions) {
  switch (dimensions) {
  case 1:
    row->array = VET;
    break;
  case 2:
    row->array = MAT;
    break;
  }
}
