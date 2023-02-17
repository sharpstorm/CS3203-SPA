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
  virtual EntityType getTypeOfSymbol(string) const = 0;
  virtual unordered_set<string> getSymbolsOfType(EntityType) const = 0;
};
