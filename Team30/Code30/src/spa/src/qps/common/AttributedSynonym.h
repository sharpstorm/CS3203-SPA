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

  PQLQuerySynonymProxy getSynProxy();
  PQLQuerySynonym* getSyn();
  PQLSynonymType getType();
  PQLSynonymName getName();
  PQLSynonymAttribute getAttribute();

  bool isType(const PQLSynonymType &type);
  bool validateAttribute();
  bool returnsInteger();
  bool isStatementType();
  bool hasAttribute();

  bool isDefaultAttribute();
};

typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;
