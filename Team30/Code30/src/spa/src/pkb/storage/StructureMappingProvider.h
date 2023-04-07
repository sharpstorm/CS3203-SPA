#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IStructureMappingProvider.h"

using std::string;
using std::unordered_set;

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(StatementStorage *, ProcedureAndCallsStorage *);
  StmtType getStatementType(int) const override;
  const StmtValueSet &getValuesOfType(StmtType) const override;
  bool isValueOfType(StmtType, StmtValue) const override;
  string getProcedureForLine(int) const override;
  string getCalledDeclaration(int) const override;

 private:
  StatementStorage *statementStorage;
  ProcedureAndCallsStorage *procAndCallsStorage;
};
