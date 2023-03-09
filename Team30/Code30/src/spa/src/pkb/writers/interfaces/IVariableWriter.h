#pragma once

#include <string>
#include "common/Types.h"

using std::string;

class IVariableWriter {
 public:
  virtual ~IVariableWriter() {}
  virtual void addVariable(string) = 0;
};
