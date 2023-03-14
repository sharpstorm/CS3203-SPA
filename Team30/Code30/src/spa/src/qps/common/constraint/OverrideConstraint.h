#pragma once

#include <string>
#include "Constraint.h"
#include "qps/common/PQLQuerySynonym.h"
#include "OverrideTransformer.h"

class OverrideConstraint : virtual public Constraint {
 private:
  string synName;
  OverrideTransformer overrideTransformer;
 public:
  OverrideConstraint(string synName, OverrideTransformer overrideTransformer) :
  synName(synName), overrideTransformer(overrideTransformer) {}
  bool applyConstraint(VariableTable varTable, OverrideTable* overrideTable) {
    overrideTable->insert({synName, overrideTransformer});
  };
};
