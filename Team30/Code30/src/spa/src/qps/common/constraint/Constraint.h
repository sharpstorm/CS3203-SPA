#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "OverrideTransformer.h"

using std::string, std::shared_ptr, std::unordered_map;

typedef unordered_map<string, OverrideTransformer> OverrideTable;
typedef unique_ptr<OverrideTable> OverrideTablePtr;

class Constraint {
 public:
  virtual ~Constraint() = default;
  virtual bool applyConstraint(VariableTable* variableTable,
                               OverrideTable* overrideTable,
                               PkbQueryHandler* queryHandler) = 0;
  virtual bool validateConstraint() = 0;
};

typedef shared_ptr<Constraint> ConstraintSPtr;
