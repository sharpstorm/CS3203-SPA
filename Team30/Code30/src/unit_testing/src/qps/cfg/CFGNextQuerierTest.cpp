#include "catch.hpp"
#include "TestCFGProvider.h"
#include "qps/cfg/CFGQuerier.h"

StmtTransitiveResult queryNext(CFGQuerier* querier, int left, int right) {
  return querier->queryNext(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right}
  );
}

TEST_CASE("Next Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  REQUIRE_FALSE(queryNext(&querier, 1, 2).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 2, 3).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 3, 4).isEmpty);

  REQUIRE(queryNext(&querier, 1, 1).isEmpty);
  REQUIRE(queryNext(&querier, 2, 2).isEmpty);
  REQUIRE(queryNext(&querier, 3, 3).isEmpty);
  REQUIRE(queryNext(&querier, 4, 4).isEmpty);

  REQUIRE(queryNext(&querier, 1, 3).isEmpty);
  REQUIRE(queryNext(&querier, 1, 4).isEmpty);
  REQUIRE(queryNext(&querier, 2, 1).isEmpty);
  REQUIRE(queryNext(&querier, 2, 4).isEmpty);
  REQUIRE(queryNext(&querier, 3, 2).isEmpty);
  REQUIRE(queryNext(&querier, 3, 1).isEmpty);
}

TEST_CASE("Next Linear (Const, _)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2});

  result = queryNext(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryNext(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{4});

  result = queryNext(&querier, 4, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("Next Linear (_, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryNext(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryNext(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2});

  result = queryNext(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{3});
}

TEST_CASE("Next Linear (_, _)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {2, 3},
      {3, 4}
  });
}

TEST_CASE("Next Multi-Cycle (Const, Const)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  REQUIRE_FALSE(queryNext(&querier, 1, 2).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 2, 3).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 3, 4).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 4, 5).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 5, 3).isEmpty);

  REQUIRE_FALSE(queryNext(&querier, 4, 6).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 6, 7).isEmpty);
  REQUIRE_FALSE(queryNext(&querier, 7, 3).isEmpty);
}

TEST_CASE("Next Multi-Cycle (Const, _)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2});

  result = queryNext(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryNext(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{4});

  result = queryNext(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5, 6});

  result = queryNext(&querier, 5, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryNext(&querier, 6, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{7});

  result = queryNext(&querier, 7, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});
}

TEST_CASE("Next Multi-Cycle (_, Const)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryNext(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryNext(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2, 5, 7});

  result = queryNext(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{3});

  result = queryNext(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{4});

  result = queryNext(&querier, 0, 6);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{4});

  result = queryNext(&querier, 0, 7);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{6});
}

TEST_CASE("Next Multi-Cycle (_, _)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGQuerier querier(&cfg);

  auto result = queryNext(&querier, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {2, 3},
      {3, 4},
      {4, 5},
      {5, 3},
      {4, 6},
      {6, 7},
      {7, 3},
  });
}
