#pragma once

#include "PQLTypes.h"
#include <unordered_map>

using std::unordered_map;

struct QueryVariable {
    PQL_VAR_TYPE type;
    PQL_VAR_NAME name;
};

typedef unordered_map<string, QueryVariable> VariableTable;
