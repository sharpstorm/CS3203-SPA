#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"
#include "VariableTable.h"

using std::unique_ptr, std::vector, std::shared_ptr, std::string;

class AttributedSynonym {
  PQLQuerySynonymProxy synProxy;
  PQLSynonymAttribute attribute;

  PQLQuerySynonym* resolveProxy();

 public:
  AttributedSynonym();
  explicit AttributedSynonym(PQLQuerySynonymProxy synProxy);
  AttributedSynonym(PQLQuerySynonymProxy synProxy, PQLSynonymAttribute attr);

  PQLQuerySynonymProxy getSynProxy();
  PQLQuerySynonym* getSyn();
  PQLSynonymType getType();
  PQLSynonymName getName();
  PQLSynonymAttribute getAttribute();
  bool validateAttribute();
  bool returnsInteger();
  bool isStatementType();
};

typedef shared_ptr<AttributedSynonym> AttributedSynonymSPtr;
typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;
