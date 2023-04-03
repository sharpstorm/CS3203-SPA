#pragma once

#include <string>
#include <set>
#include <utility>

#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IStructureMappingProvider.h"

using std::set;
using std::string;

class StructureMappingProvider : public IStructureMappingProvider {
 public:
  StructureMappingProvider(StatementStorage *, ProcedureAndCallsStorage *);
  StmtType getStatementType(int) const override;
  const StmtSet& getValuesOfType(StmtType) const override;
  bool isValueOfType(StmtType, StmtValue) const override;
  string getProcedureForLine(int) const override;
  string getCalledDeclaration(int) const override;

 private:
  StatementStorage *statementStorage;
  ProcedureAndCallsStorage *procAndCallsStorage;
};
