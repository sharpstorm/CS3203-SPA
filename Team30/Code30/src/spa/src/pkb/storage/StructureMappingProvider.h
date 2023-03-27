#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IStructureMappingProvider.h"

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(StatementStorage *, ProcedureAndCallsStorage *);
  StmtType getStatementType(int) const override;
  std::unordered_set<int> getValuesOfType(StmtType) const override;
  bool isValueOfType(StmtType, StmtValue) const override;
  std::string getProcedureForLine(int) const override;
  std::string getCalledDeclaration(int) const override;

 private:
  StatementStorage *statementStorage;
  ProcedureAndCallsStorage *procAndCallsStorage;
};
