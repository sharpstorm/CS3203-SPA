#pragma once

#include <list>
#include <string>
#include <vector>

using std::string, std::vector, std::list;

class CFG {
 public:
  explicit CFG(string n);
  CFG();
  void addNode(int lineNum1, int lineNum2);

  void increaseMapSize(int num);

  vector<int> getNodeMap();
  vector<list<int>> getLinks();

  void changeStartingLine(int num);

  string getName();

 private:
  string procedureName;
  int startingLine;
  vector<int> nodeMap;
  vector<list<int>> links;
};
