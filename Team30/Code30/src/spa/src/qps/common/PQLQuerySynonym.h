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

  PQLSynonymType getType() const;
  PQLSynonymName getName() const;
  bool isStatementType() const;
  bool isEntityType() const;
  bool isType(PQLSynonymType type) const;
  bool isSubtypeOf(const PQLQuerySynonym *other) const;
  bool isDistinctSubtypeFrom(const PQLQuerySynonym *other) const;
  bool operator==(const PQLQuerySynonym& other) const;
};

typedef vector<PQLQuerySynonym> PQLQuerySynonymList;
