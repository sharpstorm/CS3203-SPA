#pragma once

#include <memory>
#include <string>

#include "common/cfg/CFG.h"

using std::string;

class ICFGsWriter {
 public:
  virtual ~ICFGsWriter() {}
  virtual void addCFGs(string arg1, CFGSPtr arg2) = 0;
};
