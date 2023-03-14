#pragma once

#include <vector>
#include <array>

#include "common/UtilityTypes.h"

using std::vector, std::array;

typedef uint16_t CFGNode;
static const CFGNode CFG_END_NODE = 65534;

class CFGLinks {
 private:
  vector<CFGNode> links;

 public:
  CFGLinks() = default;
  explicit CFGLinks(const CFGNode &node);
  void addLink(const CFGNode &node);

  vector<CFGNode>::iterator begin();
  vector<CFGNode>::iterator end();
};
