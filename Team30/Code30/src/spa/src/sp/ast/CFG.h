#pragma once

#include <list>
#include <string>
#include <vector>

using std::string, std::vector, std::list;

class CFG {
 public:
  explicit CFG(string n);
  void addNode(int lineNum1, int lineNum2);

  void increaseMapSize(int num);

  vector<int> getNodeMap();
  vector<list<int>> getLinks();

  string getName();

 private:
  string procedureName;
  vector<int> nodeMap;
  vector<list<int>> links;
};
