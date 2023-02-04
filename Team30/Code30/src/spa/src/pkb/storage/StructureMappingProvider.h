#pragma once

#include <unordered_set>

using std::unordered_set;

#include "../../common/Types.h"
class StructureMappingProvider {
 public:
  StructureMappingProvider();

  bool isStatementOfType(int s, StmtType stmtType) const;
  unordered_set<int> getStatementsOfType(StmtType stmtType) const;
};
