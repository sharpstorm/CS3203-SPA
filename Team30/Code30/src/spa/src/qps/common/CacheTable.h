#pragma once

#include <set>
#include <vector>

#include "common/Types.h"

using std::vector, std::set;

typedef vector<StmtValue> CacheRow;

class CacheTable {
  vector<CacheRow> forwardMatrix;
  vector<CacheRow> reverseMatrix;
  set<StmtValue> fullForward;
  set<StmtValue> fullReverse;
  CacheRow *queryFrom(StmtValue stmt);
  CacheRow *queryTo(StmtValue stmt);

 public:
  bool isValidIndex(StmtValue stmt, size_t size);
  void addEntry(StmtValue leftStmt, StmtValue rightStmt);
  CacheRow *queryFull(StmtValue leftStmt, StmtValue rightStmt);
  CacheRow *queryPartial(StmtValue leftStmt, StmtValue rightStmt);
  void promoteFrom(StmtValue stmt);
  void promoteTo(StmtValue stmt);
  bool entryExists(StmtValue left, StmtValue right);
};
