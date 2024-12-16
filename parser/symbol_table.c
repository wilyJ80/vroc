#include "./symbol_table.h"

struct SymbolTable initTable() {
  struct SymbolTable table;
  table.top = -1;
  return table;
}

