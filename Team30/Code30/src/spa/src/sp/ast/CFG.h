#pragma once

#include <list>
#include <string>
#include <vector>
#include <memory>

using std::string, std::vector, std::list, std::shared_ptr;

class CFG {
 public:
  explicit CFG(string name, int start);
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

typedef shared_ptr<CFG> CFGSPtr;
