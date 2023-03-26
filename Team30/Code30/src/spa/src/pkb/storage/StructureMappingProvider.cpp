#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider(
    StatementStorage *statementStorage,
    ProcedureAndCallsStorage *procAndCallsStorage)
    : statementStorage(statementStorage),
      procAndCallsStorage(procAndCallsStorage) {}

StmtType StructureMappingProvider::getStatementType(int lineNumber) const {
  return statementStorage->getTypeOfStatement(lineNumber);
}

std::unordered_set<int> StructureMappingProvider::getValuesOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return statementStorage->getAllStatements();
  } else {
    return statementStorage->getStatementsOfType(stmtType);
  }
}

std::string StructureMappingProvider::getProcedureForLine(
    int lineNumber) const {
  return procAndCallsStorage->getProcedureForLine(lineNumber);
}

std::string StructureMappingProvider::getCalledDeclaration(
    int lineNumber) const {
  return procAndCallsStorage->getCalledDeclaration(lineNumber);
}

bool StructureMappingProvider::isValueOfType(StmtType stmtType,
                                             StmtValue lineNumber) const {
  return statementStorage->getTypeOfStatement(lineNumber) == stmtType;
}
