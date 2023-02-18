#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include "pkb/writers/PkbWriter.h"

using std::vector, std::string, std::pair;

class StubPkb : public PkbWriter {
 public:
  StubPkb(PKB* pkb) : PkbWriter(pkb) {

  }
  void addModifies(int i, string var) {
    modifiesStore.push_back(make_pair(i, var));
  }
  void addUses(int i, string var) {
    usesStore.push_back(make_pair(i, var));
  }
  void addAssigns(int i, shared_ptr<IASTNode> node) {
    patternStore.push_back(make_pair(i, node));
  }
  vector<pair<int,string>> modifiesStore;
  vector<pair<int,string>> usesStore;
  vector<pair<int, shared_ptr<IASTNode>>> patternStore;
};