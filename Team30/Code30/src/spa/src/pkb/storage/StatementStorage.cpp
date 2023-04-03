#include "StatementStorage.h"

StatementStorage::StatementStorage(
    IBaseTable<StmtValue, StmtType> *table,
    IBaseSetTable<StmtType, StmtValue> *reverseTable, StmtSet *values)
    : table(table), reverseTable(reverseTable), allStatements(values) {}

void StatementStorage::insert(StmtValue stmt, StmtType type) {
  table->insert(stmt, type);
  reverseTable->insert(type, stmt);
  allStatements->insert(stmt);
}

const StmtSet &StatementStorage::getStatementsOfType(StmtType stmtType) const {
  if (stmtType == StmtType::None || stmtType == StmtType::Wildcard) {
    return getAllStatements();
  }
  return reverseTable->get(stmtType);
}

const StmtSet &StatementStorage::getAllStatements() const {
  return *allStatements;
}

StmtType StatementStorage::getTypeOfStatement(StmtValue stmt) const {
  return table->get(stmt);
}
