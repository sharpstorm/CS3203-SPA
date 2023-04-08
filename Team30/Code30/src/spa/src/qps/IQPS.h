#pragma once

#include <string>
#include "common/UtilityTypes.h"
#include "qps/common/PQLTypes.h"
#include "qps/common/QPSTypes.h"

using std::string;

class IQPS {
 public:
  virtual ~IQPS() = default;
  virtual void evaluate(const string &query, QPSOutputList *output) = 0;
};
