#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "ProcedureStorage.h"
#include "StatementStorage.h"
#include "interfaces/IStructureMappingProvider.h"

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(StatementStorage *, ProcedureStorage *);
  StmtType getStatementType(int) const override;
  bool isStatementOfType(int, StmtType) const override;
  std::unordered_set<int> getStatementsOfType(StmtType) const override;
  std::pair<int, int> getProcedureLines(std::string) const override;
  std::string getProcedureForLine(int) const override;

 private:
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
};
