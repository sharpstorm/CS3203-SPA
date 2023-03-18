#pragma once

#include "PQLTypes.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using std::unordered_map, std::vector, std::unique_ptr;

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
  bool isStatementType();
  bool isEntityType();
  bool isType(PQLSynonymType type);
  bool operator==(const PQLQuerySynonym& other) const;
};

//typedef unordered_map<string, PQLQuerySynonym> VariableTable;
//typedef unique_ptr<VariableTable> VariableTablePtr;
typedef vector<PQLQuerySynonym> PQLQuerySynonymList;
