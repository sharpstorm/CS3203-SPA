#include "CFG.h"

#include "../../../../../lib/catch.hpp"

CFG::CFG(string procedure) : name(procedure) {
  root = CFGNodePtr(CFG_STARTING_BLOCK);
}

CFGNodePtr CFG::getMutableRoot() { return root; }

std::string CFG::getName() { return name; }
