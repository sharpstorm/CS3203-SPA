#pragma once

#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "common/Types.h"

using std::vector, std::set, std::unique_ptr;

typedef vector<StmtValue> CacheRow;

class CacheTable {
  vector<CacheRow> forwardMatrix;
  vector<CacheRow> reverseMatrix;
  set<StmtValue> fullForward;
  set<StmtValue> fullReverse;
  CacheRow* queryFrom(StmtValue stmt);
  CacheRow* queryTo(StmtValue stmt);

 public:
  void addEntry(StmtValue leftStmt, StmtValue rightStmt);
  CacheRow* queryFull(StmtValue leftStmt, StmtValue rightStmt);
  CacheRow* queryPartial(StmtValue leftStmt, StmtValue rightStmt);
  void promoteFrom(StmtValue stmt);
  void promoteTo(StmtValue stmt);
};
