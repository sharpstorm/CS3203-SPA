#pragma once

#include <utility>

#include "qps/common/IConstraint.h"
#include "qps/clauses/arguments/WithArgument.h"

class ConstantConstraint : public IConstraint {
 private:
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;
 public:
  ConstantConstraint(WithArgumentPtr arg1, WithArgumentPtr arg2);
  bool applyConstraint(SynonymProxyBuilder *variableTable,
                       OverrideTable *overrideTable) override;
  const PQLSynonymNameList getAffectedSyns() const override;
  bool validateConstraint() const override;
};
