#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "pkb/writers/PkbWriter.h"

using std::vector, std::string, std::pair, std::make_pair, std::map;
using std::unordered_set, std::unordered_map;

template<typename T1, typename T2, typename T3>
using triple = std::tuple<T1, T2, T3>;

class StubPkb : public PkbWriter {
 public:
  StubPkb(PKB* pkb) : PkbWriter(pkb) {}

  void addFollows(int x, int y) { followsStore.push_back(make_pair(x, y)); }
  void addParent(int x, int y) { parentStore.push_back(make_pair(x, y)); }
  void addModifies(int i, string var, string procedure) {
    modifiesStore.push_back(make_pair(i, var));
    modifiesPStore.push_back(make_pair(procedure, var));
  }
  void addUses(int i, string var, string procedure) {
    usesStore[i].insert(var);
    usesPStore[procedure].insert(var);
  }
  bool hasUses(int i, string var) {
    if (usesStore.find(i) == usesStore.end()) {
      return false;
    }

    auto set = usesStore.at(i);
    return set.find(var) != set.end();
  }

  bool hasUses(string procedure, string var) {
    if (usesPStore.find(procedure) == usesPStore.end()) {
      return false;
    }

    auto set = usesPStore.at(procedure);
    return set.find(var) != set.end();
  }

  bool hasUses(int i, string procedure, string var) {
    return hasUses(i, var) && hasUses(procedure, var);
  }

  int getUsesSize() {
    int ret = 0;
    for (auto it = usesStore.begin(); it != usesStore.end(); it++) {
      ret += it->second.size();
    }
    return ret;
  }

  int getUsesPSize() {
    int ret = 0;
    for (auto it = usesPStore.begin(); it != usesPStore.end(); it++) {
      ret += it->second.size();
    }
    return ret;
  }

  void addAssigns(int i, PatternTrieSPtr node) {
    patternStore.push_back(make_pair(i, node));
  }

  void addProcedure(string name, int start, int end) {
    procedureRangeStore.push_back(triple<string, int, int>(name, start, end));
  }

  void addWhilePattern(int x, string v) {
    whilePatternStore.push_back(make_pair(x, v));
  }

  void addIfPattern(int x, string v) {
    ifPatternStore.push_back(make_pair(x, v));
  }

  class UsesSet {

  };

  vector<pair<int, int>> followsStore;
  vector<pair<int, int>> parentStore;
  vector<pair<int, string>> modifiesStore;
  vector<pair<string, string>> modifiesPStore;
  map<int, unordered_set<string>> usesStore;
  unordered_map<string, unordered_set<string>> usesPStore;
  vector<pair<int, PatternTrieSPtr>> patternStore;
  vector<triple<string, int, int>> procedureRangeStore;
  vector<pair<int, string>> ifPatternStore;
  vector<pair<int, string>> whilePatternStore;
};
