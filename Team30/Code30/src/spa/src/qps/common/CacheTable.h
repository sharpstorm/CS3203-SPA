#pragma once

#include <vector>

#include "common/Types.h"

using std::vector;

typedef vector<StmtValue> CacheRow;

class CacheTable {
  typedef vector<CacheRow> CacheMatrix;
  static const CacheRow emptyRow;

  class CachePart {
   private:
    CacheMatrix matrix;
    StmtSet promoted;

   public:
    const CacheRow *query(const StmtValue stmt) const;
    void promote(const StmtValue stmt);
    bool isPromoted(const StmtValue stmt) const;
    bool isValidIndex(const StmtValue &stmt) const;
    void insert(const StmtValue &key, const StmtValue &value);
  };

  CachePart forwardCache;
  CachePart reverseCache;

 public:
  void addEntry(const StmtValue &leftStmt, const StmtValue &rightStmt);
  const CacheRow *queryFull(const StmtValue &leftStmt,
                            const StmtValue &rightStmt) const;
  const CacheRow *queryPartial(const StmtValue &leftStmt,
                               const StmtValue &rightStmt) const;
  void promoteFrom(const StmtValue &stmt);
  void promoteTo(const StmtValue &stmt);
};
