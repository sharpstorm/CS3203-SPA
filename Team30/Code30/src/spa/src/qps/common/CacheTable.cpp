#include "CacheTable.h"

void CacheTable::addEntry(StmtValue leftStmt, StmtValue rightStmt) {
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

CacheRow *CacheTable::queryFull(StmtValue leftStmt, StmtValue rightStmt) {
  if (leftStmt > forwardMatrix.size() || rightStmt > reverseMatrix.size()) {
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

void CacheTable::promoteFrom(StmtValue stmt) {
  fullForward.insert(stmt);
}

void CacheTable::promoteTo(StmtValue stmt) {
  fullReverse.insert(stmt);
}

CacheRow *CacheTable::queryFrom(StmtValue stmt) {
  if (stmt > forwardMatrix.size()) {
    return nullptr;
  }

  CacheRow* cacheRow = &forwardMatrix[stmt - 1];
  if (!cacheRow->empty()) {
    promoteFrom(stmt);
  }

  return cacheRow;
}

CacheRow *CacheTable::queryTo(StmtValue stmt) {
  if (stmt > reverseMatrix.size()) {
    return nullptr;
  }

  CacheRow* cacheRow = &reverseMatrix[stmt - 1];
  if (!cacheRow->empty()) {
    promoteTo(stmt);
  }

  return cacheRow;
}

CacheRow *CacheTable::queryPartial(StmtValue leftStmt, StmtValue rightStmt) {
  if (leftStmt > forwardMatrix.size() || rightStmt > reverseMatrix.size()) {
    return nullptr;
  }

  if (leftStmt != 0 && rightStmt != 0) {
    CacheRow* row = &forwardMatrix[leftStmt];
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
