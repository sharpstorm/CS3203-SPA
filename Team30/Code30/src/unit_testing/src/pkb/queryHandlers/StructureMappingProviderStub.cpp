#include "StructureMappingProviderStub.h"

#include "pkb/PkbTypes.h"

StructureMappingProviderStub::StructureMappingProviderStub() {}

StmtType StructureMappingProviderStub::getStatementType(int stmt) const {
  return stmtNumToType.get(stmt);
}

const StmtSet& StructureMappingProviderStub::getValuesOfType(
    StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return allStmts;
  }
  return stmtTypeToNum.get(stmtType);
}

string StructureMappingProviderStub::getProcedureForLine(int stmt) const {
  return stmtNumToProcedure.get(stmt);
}

string StructureMappingProviderStub::getCalledDeclaration(int stmt) const {
  return stmtNumToCalledProcedure.get(stmt);
}

bool StructureMappingProviderStub::isValueOfType(StmtType type,
                                                 StmtValue num) const {
  return stmtNumToType.get(num) == type;
}
