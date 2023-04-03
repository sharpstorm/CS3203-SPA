#pragma once

#include <memory>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "PQLQuerySynonymProxy.h"

using std::unique_ptr, std::vector;

class AttributedSynonym {
  PQLQuerySynonymProxy synProxy;
  PQLSynonymAttribute attribute;

 public:
  explicit AttributedSynonym(const PQLQuerySynonymProxy &synProxy);
  AttributedSynonym(const PQLQuerySynonymProxy &synProxy,
                    const PQLSynonymAttribute &attr);

  PQLQuerySynonymProxy getSynProxy() const;
  PQLSynonymType getType() const;
  PQLSynonymName getName() const;
  PQLSynonymAttribute getAttribute() const;

  bool isType(const PQLSynonymType &type) const;
  bool validateAttribute() const;
  bool returnsInteger() const;
  bool isStatementType() const;
  bool hasAttribute() const;

  bool isDefaultAttribute() const;
};

typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;
