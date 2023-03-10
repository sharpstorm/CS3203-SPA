#include <cassert>
#include "CFGLinks.h"

CFGForwardLink::CFGForwardLink(): branches({CFG_NO_NODE, CFG_NO_NODE}) {}

CFGForwardLink::CFGForwardLink(const CFGNode &node):
    branches({node, CFG_NO_NODE}) {}

void CFGForwardLink::addLink(const CFGNode &node) {
  if (branches[0] == CFG_NO_NODE) {
    branches[0] = node;
  } else if (branches[1] == CFG_NO_NODE) {
    branches[1] = node;
  } else {
    assert(1 == 2);  // This should not be reached
  }
}

ForwardLinkIterator CFGForwardLink::begin() {
  return ForwardLinkIterator(&branches[0]);
}

ForwardLinkIterator CFGForwardLink::end() {
  if (branches[0] == CFG_NO_NODE) {
    return ForwardLinkIterator(&branches[0]);
  } else if (branches[1] == CFG_NO_NODE) {
    return ForwardLinkIterator(&branches[1]);
  }
  return ForwardLinkIterator((&branches[1]) + 1);
}

CFGBackwardLink::CFGBackwardLink(const CFGNode &node): links{node} {}

void CFGBackwardLink::addLink(const CFGNode &node) {
  links.push_back(node);
}

vector<CFGNode>::iterator CFGBackwardLink::begin() {
  return links.begin();
}

vector<CFGNode>::iterator CFGBackwardLink::end() {
  return links.end();
}
