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
    pair_set<string, string>(),
    isLeft
  };
}

EntityResult EntityResultBuilder::buildEntityResult(bool isLeft,
    QueryResult<string, string> queryResult) {
  if (queryResult.isEmpty) {
    return EntityResult{};
  }
  return EntityResult{
    unordered_set<int>(),
    queryResult.secondArgVals,
    pair_set<int, string>(),
    queryResult.pairVals,
    isLeft,
  };
}

EntityResult EntityResultBuilder::buildEntityResult(
    unordered_set<string> lines) {
  return EntityResult{
    unordered_set<int>{},
    lines,
    pair_set<int, string>{},
    pair_set<string, string>{},
    false
  };
}
