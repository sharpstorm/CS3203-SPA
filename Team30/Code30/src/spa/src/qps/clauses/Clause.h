#pragma once

#include "qps/common/IntermediateResultSet.h"
#include "qps/common/PQLQuery.h"

class Clause {
 public:
  virtual IntermediateResultSet* evaluateOn();
  virtual bool validateArgTypes(VariableTable* variables);
};
