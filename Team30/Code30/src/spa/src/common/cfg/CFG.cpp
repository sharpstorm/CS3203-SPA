#include "CFG.h"

CFG::CFG(const int &start): startingLineIndex(start) {}

// if node is an if/while, the first node will signify
// the node after the if/while (-1 if it is the last node)
// ifNode: (after ifStmtList, elseStmtList)
// whileNode: (after stmtLst, stmtLst)
void CFG::addLink(const CFGNode &from, const CFGNode &to) {
  addLink(&forwardLinks, from, to);

  if (to == CFG_END_NODE) {
    endNodeBackwardLink.addLink(from);
  } else {
    addLink(&backwardLinks, to, from);
  }
}

void CFG::increaseMapSize(int newSize) {
  if (newSize <= 0 || newSize <= forwardLinks.size()) {
    return;
  }

  forwardLinks.resize(newSize);
  backwardLinks.resize(newSize);
}

bool CFG::containsStatement(const int &stmtNo) {
  return (stmtNo >= startingLineIndex)
      && (stmtNo < startingLineIndex + forwardLinks.size());
}

int CFG::getStartingStmtNumber() {
  return startingLineIndex;
}

CFGNode CFG::toCFGNode(const int &stmtNo) {
  if (stmtNo == CFG_END_NODE) {
    return (stmtNo & 0xFFFF);
  }
  return (stmtNo - startingLineIndex) & MASK_LOWER_16;  // Only take lower 16
}

int CFG::fromCFGNode(const CFGNode &node) {
  return startingLineIndex + node;
}

CFGLinks* CFG::nextLinksOf(const CFGNode &node) {
  if (!containsNode(node)) {
    return nullptr;
  }

  return &forwardLinks[node];
}

CFGLinks* CFG::reverseLinksOf(const CFGNode &node) {
  if (node == CFG_END_NODE) {
    return &endNodeBackwardLink;
  } else if (!containsNode(node)) {
    return nullptr;
  }

  return &backwardLinks[node];
}

bool CFG::containsNode(const CFGNode &node) {
  return node < forwardLinks.size();
}

int CFG::getNodeCount() {
  return forwardLinks.size();
}
