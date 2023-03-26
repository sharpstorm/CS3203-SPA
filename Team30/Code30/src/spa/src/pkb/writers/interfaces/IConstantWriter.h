#pragma once

#include <string>

#include "common/Types.h"

using std::string;

class IConstantWriter {
 public:
  virtual ~IConstantWriter() {}
  virtual EntityIdx addConstant(string) = 0;
};
