#include "StructureMappingProvider.h"

StructureMappingProvider::StructureMappingProvider() {}

StmtType StructureMappingProvider::getStatementType(int lineNumber) const {
  return statementStorage->getStatement(lineNumber);
}

bool StructureMappingProvider::isStatementOfType(int lineNumber,
                                                 StmtType stmtType) const {
  return stmtType == statementStorage->getStatement(lineNumber);
}

std::unordered_set<int> StructureMappingProvider::getStatementsOfType(
    StmtType stmtType) const {
  return statementStorage->getStatementsOfType(stmtType);
}

std::pair<int, int> StructureMappingProvider::getProcedureLines(
    std::string procedureName) const {
  return procedureStorage->getProcedure(procedureName);
}

std::string StructureMappingProvider::getProcedureForLine(
    int lineNumber) const {
  return procedureStorage->getProcedureForStatement(lineNumber);
}
