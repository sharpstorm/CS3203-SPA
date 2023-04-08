#include "StatementStorage.h"

StatementStorage::StatementStorage(
    ContiguousTable<StmtType> *table,
    HashKeySetTable<StmtType, StmtValue> *reverseTable, StmtValueSet *values)
    : table(table), reverseTable(reverseTable), allStatements(values) {}

void StatementStorage::insert(StmtValue stmt, StmtType type) {
  table->insert(stmt, type);
  reverseTable->insert(type, stmt);
  allStatements->insert(stmt);
}

const StmtValueSet &StatementStorage::getStatementsOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None || stmtType == StmtType::Wildcard) {
    return getAllStatements();
  }
  return reverseTable->get(stmtType);
}

const StmtValueSet &StatementStorage::getAllStatements() const {
  return *allStatements;
}

StmtType StatementStorage::getTypeOfStatement(StmtValue stmt) const {
  return table->get(stmt);
}
