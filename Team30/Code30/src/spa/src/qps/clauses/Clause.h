#pragma once

#include "qps/common/IntermediateResultSet.h"
#include "qps/common/PQLQuery.h"
#include "qps/common/IEvaluatable.h"

class Clause : public IEvaluatable {
 public:
  virtual IntermediateResultSet* evaluateOn();
  virtual bool validateArgTypes(VariableTable* variables);
};
