#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOL_TABLE_ROWS 255

#include <stdbool.h>

enum SCOPE { GBL, LCL };

enum TYPE { TYPE_INT, TYPE_REAL, TYPE_CHAR, TYPE_BOOL, TYPE_NA };

enum CAT { VG, VL, PRC, PAR, PRT };

enum PASS { VAL, PASS_REF, PASS_NA };

enum ZOMBIE { VIV, ZMB, ZOMBIE_NA };

enum ARRAY { VS, VET, MAT, ARRAY_NA };

union ConstValue {
  int intValue;
  double doubleValue;
  bool boolValue;
  char charValue;
};

struct Row {
  const char *lexeme;
  enum SCOPE scope;
  enum TYPE type;
  enum CAT category;
  enum PASS passage;
  enum ZOMBIE zombie;
  enum ARRAY array;
  int dim1;
  int dim2;
  bool isConst;
  union ConstValue constValue;
  int refAddress;
  int procLabel;
};

struct SymbolTable {
  struct Row row[MAX_SYMBOL_TABLE_ROWS];
  int top;
};

struct SymbolTable initTable();

#endif
