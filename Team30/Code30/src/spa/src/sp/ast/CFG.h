#pragma once

#include <list>
#include <string>
#include <vector>

using std::string, std::vector, std::list;

class CFG {
 public:
  explicit CFG(string n, int start);
  CFG();
  void addLink(int lineNum1, int lineNum2);

  void increaseMapSize(int num);

  vector<list<int>> getLinks();

  string getName();

 private:
  string procedureName;
  int startingLineIndex;

  vector<list<int>> links;
};
