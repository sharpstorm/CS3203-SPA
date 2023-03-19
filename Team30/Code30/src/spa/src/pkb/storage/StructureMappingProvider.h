#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IStructureMappingProvider.h"

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(
      StatementStorage *, ProcedureStorage *,
      CallStmtStorage *);
  StmtType getStatementType(int) const override;
  std::unordered_set<int> getValuesOfType(StmtType) const override;
  bool isValueOfType(StmtType, StmtValue) const override;
  unordered_set<int> getProcedureLines(std::string) const override;
  std::string getProcedureForLine(int) const override;
  std::unordered_set<int> getCallStmtsOfProcedure(std::string) const override;
  std::string getCalledProcedure(int) const override;

 private:
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  CallStmtStorage *callStmtStorage;
};
