#pragma once

#include "../AbstractPQLContext.h"
#include "qps/common/PQLQuerySynonym.h"

class PQLPatternClauseContext: public AbstractPQLContext {
 protected:
  PQLQuerySynonym* synonym;
  explicit PQLPatternClauseContext(PQLQuerySynonym* synonym):
      synonym(synonym) {}
};
