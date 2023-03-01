#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

using std::string, std::unordered_set, std::unordered_map;

typedef int PlanNode;
typedef unordered_set<PlanNode> PlanNodes;

class QueryGrouperIndex {
 private:
  unordered_map<string, PlanNodes> synonymMap;

 public:
  PlanNodes* getUsages(const string &key);
  void insertUsage(const string &name, PlanNode value);
};
