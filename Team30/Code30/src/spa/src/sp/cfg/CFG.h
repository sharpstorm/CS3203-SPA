#pragma once
#include <string>

#include "CFGNode.h"

using std::string, std::shared_ptr;

class CFG {
 public:
  explicit CFG(string procedure);
  CFGNodePtr getMutableRoot();
  string getName();

 private:
  CFGNodePtr root;
  string name;
};
