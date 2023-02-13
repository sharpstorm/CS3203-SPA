#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider(
    StatementStorage *statementStorage, ProcedureStorage *procedureStorage)
    : statementStorage(statementStorage), procedureStorage(procedureStorage) {}

StmtType StructureMappingProvider::getStatementType(int lineNumber) const {
  return statementStorage->getByKey(lineNumber);
}

bool StructureMappingProvider::isStatementOfType(int lineNumber,
                                                 StmtType stmtType) const {
  return stmtType == statementStorage->getByKey(lineNumber);
}

std::unordered_set<int> StructureMappingProvider::getStatementsOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return statementStorage->getAllValues();
  } else {
    return statementStorage->getByValue(stmtType);
  }
}

std::pair<int, int> StructureMappingProvider::getProcedureLines(
    std::string procedureName) const {
  return procedureStorage->getProcedure(procedureName);
}

std::string StructureMappingProvider::getProcedureForLine(
    int lineNumber) const {
  return procedureStorage->getProcedureForStatement(lineNumber);
}
