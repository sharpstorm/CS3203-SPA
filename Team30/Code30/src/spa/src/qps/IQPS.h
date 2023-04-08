#pragma once

#include "common/UtilityTypes.h"
#include "qps/common/PQLTypes.h"
#include "qps/common/QPSTypes.h"

class IQPS {
 public:
  virtual ~IQPS() = default;
  virtual void evaluate(const PQLQueryString &query, QPSOutputList *output) = 0;
};
