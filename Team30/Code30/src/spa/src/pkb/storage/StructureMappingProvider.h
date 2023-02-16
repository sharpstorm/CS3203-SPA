#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "StorageTypes.h"
#include "interfaces/IStructureMappingProvider.h"

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(StatementStorage *, ProcedureStorage *);
  StmtType getStatementType(int) const override;
  bool isStatementOfType(int, StmtType) const override;
  std::unordered_set<int> getStatementsOfType(StmtType) const override;
  unordered_set<int> getProcedureLines(std::string) const override;
  std::string getProcedureForLine(int) const override;
  function<string(int const &)> getStmtProcedureTransformer() const override;

 private:
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
};
