#pragma once

#include <string>
#include "OverrideTransformer.h"

using std::string;

typedef unordered_map<string, OverrideTransformer> OverrideTable;

class Constraint {
 public:
  virtual ~Constraint() = default;
  virtual bool applyConstraint(VariableTable* variableTable,
                               OverrideTable* overrideTable) = 0;
  virtual bool validateConstraint() = 0;
};

typedef shared_ptr<Constraint> ConstraintSPtr;
