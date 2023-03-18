#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "qps/common/VariableTable.h"
#include "OverrideTransformer.h"

using std::string, std::shared_ptr, std::unordered_map;

typedef unordered_map<string, OverrideTransformer> OverrideTable;

class Constraint {
 public:
  virtual ~Constraint() = default;
  virtual bool applyConstraint(VariableTable* variableTable,
                               OverrideTable* overrideTable) = 0;
  virtual bool validateConstraint() = 0;
};

typedef shared_ptr<Constraint> ConstraintSPtr;
