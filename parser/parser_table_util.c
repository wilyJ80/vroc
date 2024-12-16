#include "./parser_table_util.h"

void addTableRow(struct Parser *parser, struct Row row) {
  parser->symbolTable.top++;
  int index = parser->symbolTable.top;
  parser->symbolTable.row[index] = row;
}
