#include "StatementStorage.h"

void StatementStorage::insert(StmtValue stmt, StmtType type) {
  table->set(stmt, type);
  reverseTable->set(type, stmt);
  allStatements->insert(stmt);
}

StmtValueSet StatementStorage::getStatementsOfType(StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return getAllStatements();
  }
  return reverseTable->get(stmtType);
}

StmtValueSet StatementStorage::getAllStatements() const {
  return *allStatements;
}

StmtType StatementStorage::getTypeOfStatement(StmtValue stmt) const {
  return table->get(stmt);
}
