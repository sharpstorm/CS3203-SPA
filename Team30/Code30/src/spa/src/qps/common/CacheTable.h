#pragma once

#include <vector>

#include "common/Types.h"

using std::vector;

typedef vector<StmtValue> CacheRow;

class CacheTable {
  typedef vector<CacheRow> CacheMatrix;
  CacheMatrix forwardMatrix;
  CacheMatrix reverseMatrix;
  StmtValue fullForward;
  StmtValue fullReverse;

  const CacheRow *queryFrom(const StmtValue &stmt) const;
  const CacheRow *queryTo(const StmtValue &stmt) const;
  bool isValidIndex(const StmtValue &stmt, const size_t size) const;
  bool isValidArg(const StmtValue &stmt, const size_t size) const;

 public:
  void addEntry(const StmtValue &leftStmt, const StmtValue &rightStmt);
  const CacheRow *queryFull(const StmtValue &leftStmt,
                            const StmtValue &rightStmt) const;
  const CacheRow *queryPartial(const StmtValue &leftStmt,
                               const StmtValue &rightStmt) const;
  void promoteFrom(const StmtValue &stmt);
  void promoteTo(const StmtValue &stmt);
};
