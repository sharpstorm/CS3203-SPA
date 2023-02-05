#pragma once

#include <unordered_set>

using std::unordered_set;

#include "../../common/Types.h"
class StructureMappingProvider {
 public:
  StructureMappingProvider();

  virtual bool isStatementOfType(int s, StmtType stmtType) const;
  virtual unordered_set<int> getStatementsOfType(StmtType stmtType) const;
};
