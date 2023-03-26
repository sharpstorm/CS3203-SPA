#pragma once

#include <utility>
#include <vector>

#include "Constraint.h"
#include "qps/clauses/arguments/WithArgument.h"

class ConstantConstraint : public Constraint {
 private:
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;
 public:
  ConstantConstraint(WithArgumentPtr arg1, WithArgumentPtr arg2);
  bool applyConstraint(SynonymProxyBuilder* variableTable,
                       OverrideTable* overrideTable) override;
  vector<PQLSynonymName> getAffectedSyns() override;
  bool validateConstraint() override;
};
