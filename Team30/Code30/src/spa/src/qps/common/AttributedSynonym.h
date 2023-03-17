#pragma once

#include <memory>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"

using std::unique_ptr, std::vector, std::shared_ptr;

class AttributedSynonym {
  PQLQuerySynonym* syn;
  PQLSynonymAttribute attribute;
 public:
  AttributedSynonym();
  explicit AttributedSynonym(PQLQuerySynonym* syn);
  AttributedSynonym(PQLQuerySynonym* syn, PQLSynonymAttribute attr);

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
