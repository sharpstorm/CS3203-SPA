#pragma once

#include "PQLTypes.h"
#include <string>
#include <unordered_map>
#include <vector>

using std::unordered_map, std::vector;

class PQLQuerySynonym {
 private:
  PQLSynonymType type;
  PQLSynonymName name;

 public:
  PQLQuerySynonym() = default;
  PQLQuerySynonym(PQLSynonymType type, PQLSynonymName name);
  PQLQuerySynonym(const PQLQuerySynonym &copy) = default;

  PQLSynonymType getType();
  PQLSynonymName getName();
  virtual PQLSynonymAttribute getAttribute();
  bool isStatementType();
  bool isEntityType();
  bool isType(PQLSynonymType type);
  bool operator==(const PQLQuerySynonym& other) const;
};

typedef unordered_map<string, PQLQuerySynonym> VariableTable;
typedef vector<PQLQuerySynonym> PQLQuerySynonymList;
