#pragma once

#include "PQLQueryResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(PkbQueryHandler pkbQueryHandler) = 0;
};
