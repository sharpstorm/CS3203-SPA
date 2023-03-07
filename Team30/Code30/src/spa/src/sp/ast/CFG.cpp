#include "CFG.h"

CFG::CFG(string n) : procedureName(n), startingLine(0) {}

CFG::CFG() : startingLine(0) { procedureName = ""; }

// if node is an if/while, the first node will signify
// the node after the if/while (-1 if it is the last node)
// ifNode: (after stmtLst, ifStmtList, elseStmtList)
// whileNode: (after stmtLst, stmtLst)
// Last node in while stmtLst will be marked with -1 at end of linked list
void CFG::addNode(int lineNum1, int lineNum2) {
  if (nodeMap.size() < lineNum1) {
    increaseMapSize(lineNum1 - startingLine);
  }

  links[lineNum1 - startingLine - 1].push_back(lineNum2);
}

void CFG::increaseMapSize(int num) {
  int oldSize = nodeMap.size() - 1;
  if (oldSize < 0) {
    oldSize = 0;
  }
  nodeMap.resize(num);

  for (int i = oldSize; i < num; i++) {
    nodeMap[i] = i + startingLine + 1;
  }

  links.resize(num);
}

vector<int> CFG::getNodeMap() { return nodeMap; }

vector<list<int>> CFG::getLinks() { return links; }

void CFG::changeStartingLine(int num) { startingLine = num; }

string CFG::getName() { return procedureName; }
