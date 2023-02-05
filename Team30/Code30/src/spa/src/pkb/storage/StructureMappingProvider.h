#pragma once

#include <unordered_set>
#include <string>
#include <utility>
#include "StatementStorage.h"
#include "ProcedureStorage.h"
#include "../../common/Types.h"

class StructureMappingProvider {
 public:
  StructureMappingProvider();
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  StmtType getStatementType(int) const;
  virtual bool isStatementOfType(int, StmtType) const;
  std::unordered_set<int> getStatementsOfType(StmtType) const;
  std::pair<int, int> getProcedureLines(std::string) const;
  std::string getProcedureForLine(int) const;
};
