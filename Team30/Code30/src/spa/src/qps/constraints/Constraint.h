#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "qps/common/VariableTable.h"
#include "OverrideTransformer.h"
#include "qps/common/constraint/SynonymProxyBuilder.h"
#include "qps/common/constraint/OverrideTable.h"

class Constraint {
 public:
  virtual ~Constraint() = default;
  virtual bool applyConstraint(SynonymProxyBuilder* variableTable,
                               OverrideTable* overrideTable) = 0;
  virtual vector<PQLSynonymName> getAffectedSyns() = 0;
  virtual bool validateConstraint() = 0;
};

typedef shared_ptr<Constraint> ConstraintSPtr;
