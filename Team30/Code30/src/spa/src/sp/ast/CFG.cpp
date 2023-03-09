#include "CFG.h"

CFG::CFG(string name, int start)
    : procedureName(name), startingLineIndex(start) {}

CFG::CFG() {
  procedureName = "";
  startingLineIndex = 0;
}

// if node is an if/while, the first node will signify
// the node after the if/while (-1 if it is the last node)
// ifNode: (after stmtLst, ifStmtList, elseStmtList)
// whileNode: (after stmtLst, stmtLst)
// Last node in while stmtLst will be marked with -1 at end of linked list
// e.g. (4, -1), where 4 is the line number of the while statement
void CFG::addLink(int lineNum1, int lineNum2) {
  if (links.size() < (lineNum1 - startingLineIndex)) {
    increaseMapSize(lineNum1 - startingLineIndex);
  }

  links[lineNum1 - startingLineIndex - 1].push_back(lineNum2);
}

void CFG::increaseMapSize(int num) {
  int oldSize = links.size() - 1;
  if (oldSize < 0) {
    oldSize = 0;
  }

  links.resize(num);
}

vector<list<int>> CFG::getLinks() { return links; }

string CFG::getName() { return procedureName; }
