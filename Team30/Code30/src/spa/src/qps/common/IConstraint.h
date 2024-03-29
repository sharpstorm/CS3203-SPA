#pragma once

#include <memory>

#include "qps/common/VariableTable.h"
#include "qps/common/constraint/OverrideTransformer.h"
#include "qps/common/constraint/SynonymProxyBuilder.h"
#include "qps/common/constraint/OverrideTable.h"

using std::unique_ptr;

class IConstraint {
 public:
  virtual ~IConstraint() = default;
  virtual bool applyConstraint(SynonymProxyBuilder *variableTable,
                               OverrideTable *overrideTable) = 0;
  virtual PQLSynonymNameList getAffectedSyns() const = 0;
  virtual bool validateConstraint() const = 0;
};

typedef unique_ptr<IConstraint> ConstraintPtr;
