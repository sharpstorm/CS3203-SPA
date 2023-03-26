#include "StatementStorage.h"

void StatementStorage::insert(StmtValue stmt, StmtType type) {
  table->set(stmt, type);
  reverseTable->set(type, stmt);
  allStatements->insert(stmt);
}

unordered_set<StmtValue> StatementStorage::getStatementsOfType(
    StmtType type) const {
  return reverseTable->get(type);
}

unordered_set<StmtValue> StatementStorage::getAllStatements() const {
  return *allStatements;
}

StmtType StatementStorage::getTypeOfStatement(StmtValue stmt) const {
  return table->get(stmt);
}
