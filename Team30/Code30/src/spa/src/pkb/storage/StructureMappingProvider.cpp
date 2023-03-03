#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider(
    StatementStorage *statementStorage, ProcedureStorage *procedureStorage)
    : statementStorage(statementStorage), procedureStorage(procedureStorage) {}

StmtType StructureMappingProvider::getStatementType(int lineNumber) const {
  return statementStorage->getByKey(lineNumber);
}

std::unordered_set<int> StructureMappingProvider::getStatementsOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return statementStorage->getAllKeys();
  } else {
    return statementStorage->getByValue(stmtType);
  }
}

unordered_set<int> StructureMappingProvider::getProcedureLines(
    std::string procedureName) const {
  return procedureStorage->getByValue(procedureName);
}

std::string StructureMappingProvider::getProcedureForLine(
    int lineNumber) const {
  return procedureStorage->getByKey(lineNumber);
}

Transformer<int, std::string>
StructureMappingProvider::getStmtProcedureTransformer() const {
  return [this](int const stmt) { return getProcedureForLine(stmt); };
}

std::unordered_set<int> StructureMappingProvider::getCallStmtsOfProcedure(
    std::string procedureName) const {
  return callStmtStorage->getByValue(procedureName);
}

std::string StructureMappingProvider::getCalledProcedure(int lineNumber) const {
  return callStmtStorage->getByKey(lineNumber);
}
