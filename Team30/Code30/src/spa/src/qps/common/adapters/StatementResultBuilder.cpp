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

  unordered_set<int> linesSet;
  unordered_set<pair<int, int>> pairsSet;

  if (isLeft) {
    linesSet = queryResult.firstArgVals;
  } else {
    linesSet = queryResult.secondArgVals;
  }
  pairsSet = queryResult.pairVals;

  vector<pair<int, int>> linePairs = vector<pair<int, int>>(
      pairsSet.begin(), pairsSet.end());
  vector<int> lines = vector<int>(linesSet.begin(), linesSet.end());

  return StatementResult{lines, linePairs, isLeft};
}
