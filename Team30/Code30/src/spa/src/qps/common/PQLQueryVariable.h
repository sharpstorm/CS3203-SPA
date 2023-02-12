#pragma once

#include "PQLTypes.h"
#include <string>
#include <unordered_map>

using std::unordered_map;

class PQLQueryVariable {
 public:
  PQLSynonymType type;
  PQL_SYN_NAME name;
  bool isStatementType();
  bool isEntityType();
  bool isType(PQLSynonymType type);
  bool operator==(const PQLQueryVariable& other) const;
};

typedef unordered_map<string, PQLQueryVariable> VariableTable;
