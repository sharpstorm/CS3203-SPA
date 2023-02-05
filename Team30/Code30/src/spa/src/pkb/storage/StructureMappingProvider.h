#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "ProcedureStorage.h"
#include "StatementStorage.h"

class StructureMappingProvider {
 public:
  StructureMappingProvider();
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  StmtType getStatementType(int) const;
  virtual bool isStatementOfType(int, StmtType) const;
  virtual std::unordered_set<int> getStatementsOfType(StmtType) const;
  std::pair<int, int> getProcedureLines(std::string) const;
  std::string getProcedureForLine(int) const;
};
