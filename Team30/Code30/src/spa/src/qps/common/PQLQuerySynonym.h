#pragma once

#include <vector>

#include "PQLTypes.h"

class PQLQuerySynonym {
 private:
  PQLSynonymType type;
  PQLSynonymName name;

 public:
  PQLQuerySynonym(const PQLSynonymType type, const PQLSynonymName &name);
  PQLQuerySynonym(const PQLQuerySynonym &copy) = default;

  PQLSynonymType getType() const;
  PQLSynonymName getName() const;

  bool isStatementType() const;
  bool isType(PQLSynonymType type) const;
  bool isSubtypeOf(const PQLQuerySynonym *other) const;
  bool isDistinctSubtypeFrom(const PQLQuerySynonym *other) const;
  bool operator==(const PQLQuerySynonym &other) const;
};

typedef vector<PQLQuerySynonym> PQLQuerySynonymList;
typedef vector<PQLQuerySynonym *> PQLQuerySynonymRefList;
