#include "CFGLinks.h"

CFGLinks::CFGLinks(const CFGNode &node): links{node} {}

void CFGLinks::addLink(const CFGNode &node) {
  links.push_back(node);
}

vector<CFGNode>::iterator CFGLinks::begin() {
  return links.begin();
}

vector<CFGNode>::iterator CFGLinks::end() {
  return links.end();
}
