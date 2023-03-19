#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PQLQueryResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/Constraint.h"

using std::shared_ptr, std::vector, std::string;

typedef vector<PQLSynonymName> SynonymList;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                                     OverrideTable* table) = 0;
  virtual SynonymList getUsedSynonyms() = 0;
};

typedef shared_ptr<IEvaluatable> IEvaluatableSPtr;
