#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"

using std::string, std::unordered_map, std::vector;

struct QueryVariable {
  PQL_VAR_TYPE type;
  PQL_VAR_NAME name;
};

typedef unordered_map<string, QueryVariable> VariableTable;

class PQLQuery {
 private:
  VariableTable variables;
  QueryVariable resultVariable;

 public:
  PQLQuery(vector<QueryVariable> variables);
  int getVariableCount();
  QueryVariable* getVariable(string name);
};
