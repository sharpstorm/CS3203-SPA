#include "QueryPlannerIndex.h"

void QueryPlannerIndex::insertUsage(const string &name, int value) {
  if (synonymMap.find(name) != synonymMap.end()) {
    synonymMap[name].insert(value);
  } else {
    synonymMap[name] = PlanNodes{value};
  }
}

PlanNodes *QueryPlannerIndex::getUsages(const string &key) {
  if (synonymMap.find(key) == synonymMap.end()) {
    return nullptr;
  }
  return &synonymMap.at(key);
}
