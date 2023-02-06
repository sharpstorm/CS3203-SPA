#include <unordered_set>
#include <utility>
#include <vector>

#include "StatementResultBuilder.h"

using std::pair, std::unordered_set, std::vector;

StatementResult StatementResultBuilder::buildStatementResult(bool isLeft,
    QueryResult<int, int> queryResult) {
  if (queryResult.isEmpty) {
    return StatementResult{};
  }

  return StatementResult{
    isLeft ? queryResult.firstArgVals : queryResult.secondArgVals,
    queryResult.pairVals,
    isLeft
  };
}

StatementResult StatementResultBuilder::buildStatementResult(
    unordered_set<int> lines) {
  return StatementResult{lines, unordered_set<pair<int, int>>{}, false};
}
