#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PQLQueryResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::shared_ptr, std::vector, std::string;

typedef vector<string> SynonymList;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(
          PkbQueryHandler* pkbQueryHandler) = 0;
  virtual SynonymList getUsedSynonyms() = 0;
};

typedef shared_ptr<IEvaluatable> IEvaluatableSPtr;
