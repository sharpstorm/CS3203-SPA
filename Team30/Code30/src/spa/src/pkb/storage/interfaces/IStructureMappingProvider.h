#pragma once

#include <functional>
#include <string>
#include <unordered_set>
#include <utility>

#include "IProvider.h"
#include "common/Types.h"

using std::string, std::unordered_set, std::pair, std::function;

class IStructureMappingProvider : public IProvider<StmtValue, StmtType> {
 public:
  virtual ~IStructureMappingProvider() {}
  virtual StmtType getStatementType(StmtValue) const = 0;
  virtual EntityValue getProcedureForLine(StmtValue) const = 0;
  virtual EntityValue getCalledDeclaration(StmtValue) const = 0;
};
