#include "CFG.h"

CFG::CFG(string n) : procedureName(n), startingLineIndex(0) {}

CFG::CFG() : startingLineIndex(0) { procedureName = ""; }

// if node is an if/while, the first node will signify
// the node after the if/while (-1 if it is the last node)
// ifNode: (after stmtLst, ifStmtList, elseStmtList)
// whileNode: (after stmtLst, stmtLst)
// Last node in while stmtLst will be marked with -1 at end of linked list
// e.g. (4, -1), where 4 is the line number of the while statement
void CFG::addNode(int lineNum1, int lineNum2) {
  if (nodeMap.size() < (lineNum1 - startingLineIndex)) {
    increaseMapSize(lineNum1 - startingLineIndex);
  }

  links[lineNum1 - startingLineIndex - 1].push_back(lineNum2);
}

void CFG::increaseMapSize(int num) {
  int oldSize = nodeMap.size() - 1;
  if (oldSize < 0) {
    oldSize = 0;
  }
  if ((oldSize + 1) > num) {
    return;
  }

  nodeMap.resize(num);

  for (int i = oldSize; i < num; i++) {
    nodeMap[i] = i + startingLineIndex + 1;
  }

  links.resize(num);
}

vector<int> CFG::getNodeMap() { return nodeMap; }

vector<list<int>> CFG::getLinks() { return links; }

void CFG::changeStartingLine(int num) { startingLineIndex = num; }

string CFG::getName() { return procedureName; }
