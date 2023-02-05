#pragma once

#include <unordered_set>
#include <utility>

#include "common/Types.h"
#include "qps/common/PQLQueryResult.h"

using std::pair, std::unordered_set;

class StatementResultBuilder {
 public:
  static StatementResult buildStatementResult(bool isLeft,
                                              QueryResult<int, int> queryResult);
};
