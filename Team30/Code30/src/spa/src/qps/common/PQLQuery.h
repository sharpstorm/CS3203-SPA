#pragma once

#include <string>
#include <unordered_map>

#include "PQLTypes.h"

using std::string, std::unordered_map;

struct QueryVariable {
  PQL_VAR_TYPE type;
  PQL_VAR_NAME name;
};

typedef unordered_map<string, QueryVariable> VariableTable;

class PQLQuery {
  VariableTable variables;
  QueryVariable resultVariable;
};
