#include "CacheTable.h"

void CacheTable::addEntry(const StmtValue &leftStmt,
                          const StmtValue &rightStmt) {
  // Insertion to fwd matrix
  if (leftStmt >= forwardMatrix.size()) {
    for (size_t i = forwardMatrix.size(); i < leftStmt; i++) {
      forwardMatrix.emplace_back();
    }
  }

  forwardMatrix[leftStmt - 1].push_back(rightStmt);

  // Insertion to reverse matrix
  if (rightStmt >= reverseMatrix.size()) {
    for (size_t i = reverseMatrix.size(); i < rightStmt; i++) {
      reverseMatrix.emplace_back();
    }
  }

  reverseMatrix[rightStmt - 1].push_back(leftStmt);
}

const CacheRow *CacheTable::queryFull(const StmtValue &leftStmt,
                                      const StmtValue &rightStmt) const {
  if (!isValidArg(leftStmt, forwardMatrix.size()) ||
      !isValidArg(rightStmt, reverseMatrix.size())) {
    return nullptr;
  }

  if (leftStmt != 0 && fullForward.find(leftStmt) != fullForward.end()) {
    return &forwardMatrix[leftStmt - 1];
  }

  if (rightStmt != 0 && fullReverse.find(rightStmt) != fullReverse.end()) {
    return &reverseMatrix[rightStmt - 1];
  }

  return nullptr;
}

void CacheTable::promoteFrom(const StmtValue &stmt) {
  fullForward.insert(stmt);
}

void CacheTable::promoteTo(const StmtValue &stmt) {
  fullReverse.insert(stmt);
}

const CacheRow *CacheTable::queryFrom(const StmtValue &stmt) const {
  if (!isValidIndex(stmt, forwardMatrix.size())) {
    return nullptr;
  }

  return &forwardMatrix[stmt - 1];
}

const CacheRow *CacheTable::queryTo(const StmtValue &stmt) const {
  if (!isValidIndex(stmt, reverseMatrix.size())) {
    return nullptr;
  }

  return &reverseMatrix[stmt - 1];
}

const CacheRow *CacheTable::queryPartial(const StmtValue &leftStmt,
                                         const StmtValue &rightStmt) const {
  if (!isValidArg(leftStmt, forwardMatrix.size()) ||
      !isValidArg(rightStmt, reverseMatrix.size())) {
    return nullptr;
  }

  if (leftStmt != 0 && rightStmt != 0) {
    const CacheRow *row = &forwardMatrix[leftStmt - 1];
    for (auto r : *row) {
      if (r == rightStmt) {
        return row;
      }
    }

    return nullptr;
  }

  if (leftStmt == 0) {
    return queryTo(rightStmt);
  }

  return queryFrom(leftStmt);
}

bool CacheTable::isValidIndex(const StmtValue &stmt,
                              const size_t &size) const {
  return 0 < stmt && stmt <= size;
}

bool CacheTable::isValidArg(const StmtValue &stmt,
                            const size_t &size) const {
  return 0 == stmt || isValidIndex(stmt, size);
}
