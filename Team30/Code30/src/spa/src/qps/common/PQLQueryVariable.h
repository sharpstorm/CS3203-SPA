#pragma once

#include "PQLTypes.h"
#include <string>
#include <unordered_map>

using std::unordered_map;

class PQLQueryVariable {
 public:
  PQLSynonymType type;
  PQL_VAR_NAME name;
  bool isStatementType();
  bool isType(PQLSynonymType type);
  bool operator==(const PQLQueryVariable& other) const;
};

typedef unordered_map<string, PQLQueryVariable> VariableTable;
