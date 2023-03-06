#include "CFG.h"

CFG::CFG(string n) : procedureName(n) {}

void CFG::addNode(int lineNum1, int lineNum2) {
  if (nodeMap.size() < lineNum1) {
    increaseMapSize(lineNum1);
  }

  links[lineNum1 - 1].push_back(lineNum2);
}

void CFG::increaseMapSize(int num) {
  int oldSize = nodeMap.size() - 1;
  nodeMap.resize(num);

  for (int i = oldSize; i < num; i++) {
    nodeMap[i] = i + 1;
  }

  links.resize(num);
}

vector<int> CFG::getNodeMap() { return nodeMap; }

vector<list<int>> CFG::getLinks() { return links; }
