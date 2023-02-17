#include "StructureMappingProviderStub.h"
#include "pkb/PkbTypes.h"

StructureMappingProviderStub::StructureMappingProviderStub() {};

StmtType StructureMappingProviderStub::getStatementType(int stmt) const {
  return stmtNumToType.get(stmt);
}

unordered_set<int> StructureMappingProviderStub::getStatementsOfType(StmtType stmtType) const {
  if (stmtType == StmtType::None) {
    return allStmts;
  }
  return stmtTypeToNum.get(stmtType);
};

unordered_set<int> StructureMappingProviderStub::getProcedureLines(string procedure) const {
  return procedureToStmtNum.get(procedure);
};

string StructureMappingProviderStub::getProcedureForLine(int stmt) const {
  return stmtNumToProcedure.get(stmt);
};

Transformer<int, string>
StructureMappingProviderStub::getStmtProcedureTransformer() const {
  return [this](int const stmt) { return getProcedureForLine(stmt); };
};
