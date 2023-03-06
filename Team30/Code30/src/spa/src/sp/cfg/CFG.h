#pragma once
#include "CFGNode.h"

class CFG {
 public:
  explicit CFG(CFGNodePtr node);
  CFGNodePtr getMutableRoot();

 private:
  CFGNodePtr root;
};
