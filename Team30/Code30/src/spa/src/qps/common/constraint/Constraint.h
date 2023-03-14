#pragma once

#include <string>
#include "OverrideTransformer.h"
#include "qps/common/PQLQuery.h"

using std::string;

class Constraint {
 public:
  Constraint;
  virtual ~Constraint() = default;
  virtual bool applyConstraint(VariableTable* variableTable, OverrideTable* overrideTable) = 0;
};
