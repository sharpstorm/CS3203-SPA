#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"
#include "VariableTable.h"
#include "PQLQuerySynonymProxy.h"

using std::unique_ptr, std::vector, std::string;

class AttributedSynonym {
  PQLQuerySynonymProxy synProxy;
  PQLSynonymAttribute attribute;

 public:
  explicit AttributedSynonym(PQLQuerySynonymProxy synProxy);
  AttributedSynonym(PQLQuerySynonymProxy synProxy, PQLSynonymAttribute attr);

  PQLQuerySynonymProxy getSynProxy() const;
  PQLQuerySynonym* getSyn() const;
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
