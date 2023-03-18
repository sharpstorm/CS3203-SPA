#pragma once

#include <string>
#include <unordered_set>

#include "common/Types.h"

using std::string;
using std::unordered_set;

class IDesignEntitiesQueryHandler {
 public:
  virtual ~IDesignEntitiesQueryHandler() {}
  virtual unordered_set<string> getValuesOfType(EntityType) const = 0;
  virtual unordered_set<int> getValuesOfType(StmtType) const = 0;
  virtual StmtType getStatementType(int) const = 0;
  virtual string getVariableByIndex(int) const = 0;
  virtual string getConstantByIndex(int) const = 0;
  virtual unordered_set<int> getIndexOfVariable(string) const = 0;
  virtual unordered_set<int> getIndexOfConstant(string) const = 0;
  virtual bool isStatementOfType(StmtType, int) const = 0;
  virtual bool isSymbolOfType(EntityType, string) const = 0;
  virtual string getCalledDeclaration(int) const = 0;
};
