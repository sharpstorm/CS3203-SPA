#include "CFGNode.h"

CFGNode::CFGNode(CFGNodeType t) : type(t) {}

CFGNode::CFGNode(CFGNodeType t, int num) : type(t), lineNumber(num) {}

void CFGNode::addChild(CFGNodePtr node) { children.push_back(node); }

void CFGNode::setChild(int index, CFGNodePtr node) { children[index] = node; }

shared_ptr<CFGNode> CFGNode::getChild(int index) {
  if (index < 0 || index >= children.size()) {
    return nullptr;
  }

  return children[index];
}

CFGNodeType CFGNode::getType() { return type; }
