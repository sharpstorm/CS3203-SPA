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
  if (!forwardCache.isValidArg(leftStmt)
      || !reverseCache.isValidArg(rightStmt)) {
    return nullptr;
  }

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
  if (!forwardCache.isValidArg(leftStmt)
      || !reverseCache.isValidArg(rightStmt)) {
    return nullptr;
  }

  if (leftStmt != 0 && rightStmt != 0) {
    const CacheRow *row = forwardCache.query(leftStmt);

    auto it = std::find(row->begin(), row->end(), rightStmt);
    if (it == row->end()) {
      return nullptr;
    }

    return row;
  }

  if (leftStmt == 0) {
    return reverseCache.query(rightStmt);
  }

  return forwardCache.query(leftStmt);
}

const CacheRow *CacheTable::CachePart::query(const StmtValue stmt) const {
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

bool CacheTable::CachePart::isValidArg(const StmtValue &stmt) const {
  return 0 == stmt || isValidIndex(stmt);
}

void CacheTable::CachePart::insert(const StmtValue &key,
                                   const StmtValue &value) {
  for (size_t i = matrix.size(); i < key; i++) {
    matrix.emplace_back();
  }

  matrix[key - 1].push_back(value);
}
