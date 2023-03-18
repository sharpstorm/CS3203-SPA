#include "StructureMappingProviderStub.h"

#include "pkb/PkbTypes.h"

StructureMappingProviderStub::StructureMappingProviderStub() {};

StmtType StructureMappingProviderStub::getStatementType(int stmt) const {
  return stmtNumToType.get(stmt);
}

unordered_set<int> StructureMappingProviderStub::getValuesOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return allStmts;
  }
  return stmtTypeToNum.get(stmtType);
}

unordered_set<int> StructureMappingProviderStub::getProcedureLines(
    string procedure) const {
  return procedureToStmtNum.get(procedure);
}

string StructureMappingProviderStub::getProcedureForLine(int stmt) const {
  return stmtNumToProcedure.get(stmt);
}

string StructureMappingProviderStub::getCalledProcedure(int stmt) const {
  return stmtNumToCalledProcedure.get(stmt);
}

unordered_set<int> StructureMappingProviderStub::getCallStmtsOfProcedure(
    string procedureName) const {
  return CalledProcedureToStmtNum.get(procedureName);
}

bool StructureMappingProviderStub::isStatementOfType(StmtType stmtType,
                                                     int lineNumber) const {
  return stmtNumToType.get(lineNumber) == stmtType;
}
