#pragma once

#include <memory>
#include <vector>

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"

using std::unique_ptr, std::vector;

class AttributedSynonym : public PQLQuerySynonym {
  PQLSynonymAttribute attribute;
 public:
  AttributedSynonym();
  explicit AttributedSynonym(const PQLQuerySynonym& syn);
  AttributedSynonym(const PQLQuerySynonym& syn, PQLSynonymAttribute attr);

  PQLSynonymAttribute getAttribute();
  bool validateAttribute();
};

typedef unique_ptr<AttributedSynonym> AttributedSynonymPtr;
typedef vector<AttributedSynonym> AttributedSynonymList;
