#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../../common/Types.h"

using std::string;
using std::unordered_set;

class IEntityMappingProvider {
 public:
  virtual ~IEntityMappingProvider() {}
  virtual unordered_set<string> getSymbolsOfType(EntityType) const = 0;
  virtual string getVariableByIndex(int) const = 0;
  virtual string getConstantByIndex(int) const = 0;
  virtual unordered_set<int> getIndexOfVariable(string) const = 0;
  virtual unordered_set<int> getIndexOfConstant(string) const = 0;
  virtual bool isSymbolOfType(EntityType, string) const = 0;
};
