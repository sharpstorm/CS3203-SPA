#pragma once

#include <memory>

#include "PQLQueryResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::shared_ptr;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(
          shared_ptr<PkbQueryHandler> pkbQueryHandler) = 0;
  virtual bool usesSynonym(string varName) = 0;
};
