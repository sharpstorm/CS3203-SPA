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
  virtual StmtType getStatementType(int) const = 0;
  virtual string getProcedureForLine(int) const = 0;
  virtual string getCalledDeclaration(int) const = 0;
};
