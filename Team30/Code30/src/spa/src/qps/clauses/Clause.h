#pragma once

#include "qps/common/QueryResult.h"
#include "qps/common/PQLQuery.h"
#include "qps/common/IEvaluatable.h"

class Clause : public IEvaluatable {
 public:
  virtual ~Clause() = default;
  virtual bool validateArgTypes(VariableTable* variables) = 0;
};
