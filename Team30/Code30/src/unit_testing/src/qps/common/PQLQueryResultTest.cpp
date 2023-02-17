#include "catch.hpp"
#include <unordered_set>

#include "qps/common/PQLQueryResult.h"

using std::unordered_set;

PQLSynonymName TEST_VAR_NAME = "a";
PQLSynonymName TEST_NOT_EXIST_VAR_NAME = "z";

pair_set<int, int> LINE_PAIRS = pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}});
unordered_set<int> STATEMENT_LINES = unordered_set<int>({1, 2, 3, 4});

pair_set<int, string> ENTITY_PAIRS = pair_set<int, string>({ {1, "x"}, {1, "y"}});
unordered_set<int> ENTITY_LINES = unordered_set<int>({1, 2});
unordered_set<string> ENTITIES = unordered_set<string>({"x", "y"});

StatementResult buildStatementResult() {
  StatementResult result;
  result.isLeftArg = true;
  result.linePairs = LINE_PAIRS;
  result.lines = STATEMENT_LINES;
  return result;
}

EntityResult buildEntityResult() {
  EntityResult result;
  result.isLeftArg = true;
  result.entityPairs = ENTITY_PAIRS;
  result.lines = ENTITY_LINES;
  result.entities = ENTITIES;
  return result;
}
