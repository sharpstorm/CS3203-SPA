#include "CacheTable.h"
#include <algorithm>

using std::find;

void CacheTable::addEntry(const StmtValue &leftStmt,
                          const StmtValue &rightStmt) {
  forwardCache.insert(leftStmt, rightStmt);
  reverseCache.insert(rightStmt, leftStmt);
}

const CacheRow *CacheTable::queryFull(const StmtValue &leftStmt,
                                      const StmtValue &rightStmt) const {
  if (leftStmt != 0 && forwardCache.isPromoted(leftStmt)) {
    return forwardCache.query(leftStmt);
  }

  if (rightStmt != 0 && reverseCache.isPromoted(rightStmt)) {
    return reverseCache.query(rightStmt);
  }

  return nullptr;
}

void CacheTable::promoteFrom(const StmtValue &stmt) {
  forwardCache.promote(stmt);
}

void CacheTable::promoteTo(const StmtValue &stmt) {
  reverseCache.promote(stmt);
}

const CacheRow *CacheTable::queryPartial(const StmtValue &leftStmt,
                                         const StmtValue &rightStmt) const {
  if (!forwardCache.isValidIndex(leftStmt)
      || !reverseCache.isValidIndex(rightStmt)) {
    return nullptr;
  }

  const CacheRow *row = forwardCache.query(leftStmt);

  auto it = std::find(row->begin(), row->end(), rightStmt);
  if (it != row->end()) {
    return row;
  }

  return nullptr;
}

const CacheRow *CacheTable::CachePart::query(const StmtValue stmt) const {
  if (stmt - 1 < 0 || stmt - 1 >= matrix.size()) {
    return &emptyRow;
  }
  return &matrix[stmt - 1];
}

bool CacheTable::CachePart::isPromoted(const StmtValue stmt) const {
  return promoted.find(stmt) != promoted.end();
}

void CacheTable::CachePart::promote(const StmtValue stmt) {
  promoted.insert(stmt);
}

bool CacheTable::CachePart::isValidIndex(const StmtValue &stmt) const {
  return 0 < stmt && stmt <= matrix.size();
}

void CacheTable::CachePart::insert(const StmtValue &key,
                                   const StmtValue &value) {
  for (size_t i = matrix.size(); i < key; i++) {
    matrix.emplace_back();
  }

  matrix[key - 1].push_back(value);
}

const CacheRow CacheTable::emptyRow = {};
