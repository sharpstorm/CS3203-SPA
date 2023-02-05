#pragma once

#include "qps/common/IEvaluatable.h"
#include "qps/common/PQLQueryResult.h"

class Clause : public IEvaluatable {
 public:
  virtual ~Clause() = default;
  virtual bool validateArgTypes(VariableTable* variables) = 0;
};
