#include <unordered_set>
#include <utility>
#include <vector>

#include "EntityResultBuilder.h"

using std::pair, std::unordered_set, std::vector;

EntityResult EntityResultBuilder::buildEntityResult(bool isLeft,
    QueryResult<int, string> queryResult) {
  if (queryResult.isEmpty) {
    return EntityResult{};
  }

  return EntityResult{
    queryResult.firstArgVals,
    queryResult.secondArgVals,
    queryResult.pairVals,
    isLeft
  };
}

EntityResult EntityResultBuilder::buildEntityResult(
    unordered_set<string> lines) {
  return EntityResult{
    unordered_set<int>{},
    lines,
    unordered_set<pair<int, string>>{},
    false
  };
}
