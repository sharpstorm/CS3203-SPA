#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"

using std::unique_ptr, std::vector, std::shared_ptr, std::string;

class AttributedSynonym {
  PQLQuerySynonym* syn;
  PQLSynonymAttribute attribute;
 public:
  AttributedSynonym();
  explicit AttributedSynonym(PQLQuerySynonym* syn);
  AttributedSynonym(PQLQuerySynonym* syn, PQLSynonymAttribute attr);

  PQLQuerySynonym* getSyn();
  PQLSynonymType getType();
  PQLSynonymName getName();
  PQLSynonymAttribute getAttribute();
  PQLQuerySynonym* getSyn();
  bool validateAttribute();
  bool returnsInteger();
  bool isStatementType();
};

typedef shared_ptr<AttributedSynonym> AttributedSynonymSPtr;
typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;
