#pragma once

#include "PQLTypes.h"
#include <string>
#include <unordered_map>

using std::unordered_map;

struct PQLQueryVariable {
    PQLSynonymType type;
    PQL_VAR_NAME name;
};

typedef unordered_map<string, PQLQueryVariable> VariableTable;
