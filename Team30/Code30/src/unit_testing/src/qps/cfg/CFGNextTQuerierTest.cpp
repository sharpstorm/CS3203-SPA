#include "catch.hpp"
#include "TestCFGProvider.h"
#include "qps/cfg/CFGQuerier.h"

StmtTransitiveResult queryNextT(CFGQuerier* querier, int left, int right) {
  return querier->queryNextT(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right}
  );
}

void assertQueryNextTEmpty(CFGQuerier* querier,
                           int left,
                           unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE(queryNextT(querier, left, *it).isEmpty);
  }
}

void assertQueryNextTNotEmpty(CFGQuerier* querier,
                              int left,
                              unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE_FALSE(queryNextT(querier, left, *it).isEmpty);
  }
}

TEST_CASE("NextT Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  assertQueryNextTNotEmpty(&querier, 1, {2, 3, 4});
  assertQueryNextTNotEmpty(&querier, 2, {3, 4});
  assertQueryNextTNotEmpty(&querier, 3, {4});

  assertQueryNextTEmpty(&querier, 1, {1});
  assertQueryNextTEmpty(&querier, 2, {1, 2});
  assertQueryNextTEmpty(&querier, 3, {1, 2, 3});
  assertQueryNextTEmpty(&querier, 4, {1, 2, 3, 4});
}

TEST_CASE("NextT Linear (Const, _)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3, 4});

  result = queryNextT(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4});

  result = queryNextT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{4});

  result = queryNextT(&querier, 4, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("NextT Linear (_, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryNextT(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryNextT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2});

  result = queryNextT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3});
}

TEST_CASE("NextT Linear (_, _)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {1, 3},
      {1, 4},
      {2, 3},
      {2, 4},
      {3, 4}
  });
}

TEST_CASE("NextT Multi-Cycle (Const, Const)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  assertQueryNextTNotEmpty(&querier, 1, {2, 3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 2, {3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 3, {3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 4, {3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 5, {3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 6, {3, 4, 5, 6, 7});
  assertQueryNextTNotEmpty(&querier, 7, {3, 4, 5, 6, 7});

  assertQueryNextTEmpty(&querier, 1, {1});
  assertQueryNextTEmpty(&querier, 2, {1, 2});
  assertQueryNextTEmpty(&querier, 3, {1, 2});
  assertQueryNextTEmpty(&querier, 4, {1, 2});
  assertQueryNextTEmpty(&querier, 5, {1, 2});
  assertQueryNextTEmpty(&querier, 6, {1, 2});
  assertQueryNextTEmpty(&querier, 7, {1, 2});
}

TEST_CASE("NextT Multi-Cycle (Const, _)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3, 4, 5, 6, 7});

  result = queryNextT(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});

  result = queryNextT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});

  result = queryNextT(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});

  result = queryNextT(&querier, 5, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});

  result = queryNextT(&querier, 6, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});

  result = queryNextT(&querier, 7, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5, 6, 7});
}

TEST_CASE("NextT Multi-Cycle (_, Const)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryNextT(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryNextT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3, 4, 5, 6, 7});

  result = queryNextT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3, 4, 5, 6, 7});

  result = queryNextT(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3, 4, 5, 6, 7});

  result = queryNextT(&querier, 0, 6);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3, 4, 5, 6, 7});

  result = queryNextT(&querier, 0, 7);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2, 3, 4, 5, 6, 7});
}

TEST_CASE("NextT Multi-Cycle (_, _)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNextT(&querier, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},
      {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7},
      {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7},
      {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7},
      {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7},
      {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7},
      {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7},
  });
}
