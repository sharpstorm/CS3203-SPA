#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider(
    StatementStorage* statementStorage,
    ProcedureAndCallsStorage* procAndCallsStorage)
    : statementStorage(statementStorage),
      procAndCallsStorage(procAndCallsStorage) {}

StmtType StructureMappingProvider::getStatementType(
    StmtValue lineNumber) const {
  return statementStorage->getTypeOfStatement(lineNumber);
}

const StmtValueSet& StructureMappingProvider::getValuesOfType(
    StmtType stmtType) const {
  return statementStorage->getStatementsOfType(stmtType);
}

EntityValue StructureMappingProvider::getProcedureForLine(
    StmtValue lineNumber) const {
  return procAndCallsStorage->getProcedureForLine(lineNumber);
}

EntityValue StructureMappingProvider::getCalledDeclaration(
    StmtValue lineNumber) const {
  return procAndCallsStorage->getCalledDeclaration(lineNumber);
}

bool StructureMappingProvider::isValueOfType(StmtType stmtType,
                                             StmtValue lineNumber) const {
  return statementStorage->getTypeOfStatement(lineNumber) == stmtType;
}
