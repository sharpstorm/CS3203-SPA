#include "CFG.h"

CFG::CFG(CFGNodePtr node) : root(node) {}

CFGNodePtr CFG::getMutableRoot() { return root; }
