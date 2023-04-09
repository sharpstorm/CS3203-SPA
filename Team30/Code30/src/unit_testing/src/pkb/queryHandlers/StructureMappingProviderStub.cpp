#include "StructureMappingProviderStub.h"

#include "pkb/PkbTypes.h"

StructureMappingProviderStub::StructureMappingProviderStub() {}

StmtType StructureMappingProviderStub::getStatementType(StmtValue stmt) const {
  return stmtNumToType.get(stmt);
}

const StmtValueSet& StructureMappingProviderStub::getValuesOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None || stmtType == StmtType::Wildcard) {
    return allStmts;
  }
  return stmtTypeToNum.get(stmtType);
}

EntityValue StructureMappingProviderStub::getProcedureForLine(
    StmtValue stmt) const {
  return stmtNumToProcedure.get(stmt);
}

EntityValue StructureMappingProviderStub::getCalledDeclaration(
    StmtValue stmt) const {
  return stmtNumToCalledProcedure.get(stmt);
}

bool StructureMappingProviderStub::isValueOfType(StmtType type,
                                                 StmtValue num) const {
  return stmtNumToType.get(num) == type;
}

void StructureMappingProviderStub::insert(StmtValue stmt, StmtType type) {
  stmtTypeToNum.insert(type, stmt);
  stmtNumToType.insert(stmt, type);
}
