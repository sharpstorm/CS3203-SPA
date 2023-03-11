#include "CFG.h"

CFG::CFG(const string &name, const int &start)
    : procedureName(name), startingLineIndex(start) {}

CFG::CFG() : CFG("", 0) {}

// if node is an if/while, the first node will signify
// the node after the if/while (-1 if it is the last node)
// ifNode: (after ifStmtList, elseStmtList)
// whileNode: (after stmtLst, stmtLst)
void CFG::addLink(const int &from, const int &to) {
  addLink(&forwardLinks, from, to);

  if (to == CFG_END_NODE) {
    int fromIndex = from - startingLineIndex;
    endNodeBackwardLink.addLink(fromIndex);
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

bool CFG::contains(const CFGNode &node) {
  return (node >= startingLineIndex)
      && (node < startingLineIndex + forwardLinks.size());
}

CFGNode CFG::getStartingNode() {
  return startingLineIndex;
}

CFGForwardLink* CFG::nextLinksOf(CFGNode node) {
  if (!contains(node)) {
    return nullptr;
  }

  int index = node - startingLineIndex;
  return &forwardLinks[index];
}

CFGBackwardLink* CFG::reverseLinksOf(CFGNode node) {
  if (node == CFG_END_NODE) {
    return &endNodeBackwardLink;
  } else if (!contains(node)) {
    return nullptr;
  }

  int index = node - startingLineIndex;
  return &backwardLinks[index];
}
