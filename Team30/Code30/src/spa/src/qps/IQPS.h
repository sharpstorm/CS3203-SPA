#pragma once

#include <string>
#include "common/UtilityTypes.h"

using std::string;

class IQPS {
 public:
  virtual ~IQPS() = default;
  virtual UniqueVectorPtr<string> evaluate(const string &query) = 0;
};
