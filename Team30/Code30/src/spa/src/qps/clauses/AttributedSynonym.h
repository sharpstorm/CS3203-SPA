#pragma once

#include "qps/common/PQLTypes.h"
#include "qps/common/PQLQuerySynonym.h"

class AttributedSynonym : public PQLQuerySynonym {
  PQLSynonymAttribute attribute;
 public:
  AttributedSynonym();
  AttributedSynonym(PQLSynonymName name, PQLSynonymType type,
                    PQLSynonymAttribute attribute);
  AttributedSynonym(const PQLQuerySynonym& syn, PQLSynonymAttribute attr);

  PQLSynonymAttribute getAttribute() override;
  bool validateAttribute();
};