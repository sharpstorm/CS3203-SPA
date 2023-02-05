#pragma once

#include <unordered_set>
#include <utility>

#include "common/Types.h"
#include "qps/common/PQLQueryResult.h"

using std::pair, std::unordered_set;

class StatementResultBuilder {
 public:
  static StatementResult buildStatementResult(unordered_set<int> linesSet,
                                              unordered_set<pair<int, int>>
                                              pairsSet);
};
