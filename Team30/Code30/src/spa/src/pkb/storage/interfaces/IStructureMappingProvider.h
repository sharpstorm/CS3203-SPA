#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../../common/Types.h"

using std::string;
using std::unordered_set;
using std::pair;

class IStructureMappingProvider {
 public:
  virtual ~IStructureMappingProvider() {}
  virtual StmtType getStatementType(int) const = 0;
  virtual bool isStatementOfType(int, StmtType) const = 0;
  virtual unordered_set<int> getStatementsOfType(StmtType) const = 0;
  virtual unordered_set<int> getProcedureLines(string) const = 0;
  virtual string getProcedureForLine(int) const = 0;
};
