#include "PQLQuery.h"

PQLQuery::PQLQuery(vector<QueryVariable> variables) {
  for (int i = 0; i < variables.size(); i++) {
    QueryVariable variable = variables.at(i);
    this->variables[variable.name] = variable;
  }
}

int PQLQuery::getVariableCount() {
  return variables.size();
}

QueryVariable *PQLQuery::getVariable(string name) {
  auto item = variables.find(name);
  if (item == variables.end()) {
    return nullptr;
  }

  return &item->second;
}
