#pragma once

#include <vector>
#include <array>

#include "common/UtilityTypes.h"

using std::vector, std::array;

typedef uint16_t CFGNode;
const static CFGNode CFG_END_NODE = 65534;
const static CFGNode CFG_NO_NODE = 65535;

typedef AbstractIterator<CFGNode> ForwardLinkIterator;

// The maximum Fan-Out is only 2
class CFGForwardLink {
 private:
  array<CFGNode, 2> branches;

 public:
  CFGForwardLink();
  explicit CFGForwardLink(const CFGNode &node);
  void addLink(const CFGNode &node);

  ForwardLinkIterator begin();
  ForwardLinkIterator end();
};

class CFGBackwardLink {
 private:
  vector<CFGNode> links;

 public:
  CFGBackwardLink() = default;
  explicit CFGBackwardLink(const CFGNode &node);
  void addLink(const CFGNode &node);

  vector<CFGNode>::iterator begin();
  vector<CFGNode>::iterator end();
};
