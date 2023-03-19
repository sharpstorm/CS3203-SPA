#include "catch.hpp"

#include "TestCFGProvider.h"
#include "CFGTestUtils.h"
#include "qps/cfg/cfg_querier/CFGNextQuerier.h"

typedef CFGNextQuerier<int, CFGTestUtils::dummyTypePredicate> CFGTestNextQuerier;

template <typename T, StmtTypePredicate<T> U>
StmtTransitiveResult queryNext(CFGNextQuerier<T, U>* querier,
                               int left, int right) {
  return querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right}
  );
}

template <typename T, StmtTypePredicate<T> U>
void queryNext(CFGNextQuerier<T, U>* querier,
               StmtTransitiveResult* result,
               int left, int right) {
  querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right},
      result
  );
}

template <typename T, StmtTypePredicate<T> U>
StmtTransitiveResult queryNext(CFGNextQuerier<T, U>* querier,
                               StmtRef left,
                               StmtRef right) {
  return querier->queryArgs(left, right);
}

template <typename T, StmtTypePredicate<T> U>
void queryNext(CFGNextQuerier<T, U>* querier,
               StmtTransitiveResult* result,
               StmtRef left,
               StmtRef right) {
  querier->queryArgs(left, right, result);
}

TEST_CASE("Next Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

  StmtTransitiveResult result;
  queryNext(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {2, 3},
      {3, 4}
  });
}

TEST_CASE("Next Multi-Cycle (Const, Const)") {
  auto cfg = TestCFGProvider::getSimpleMultiCycleCFG();
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

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
  CFGTestNextQuerier querier(&cfg, 0);

  StmtTransitiveResult result;
  queryNext(&querier, &result, 0, 0);
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

TEST_CASE("Next Type Filtering") {
  constexpr StmtTypePredicate<int> typePredicate =
      [](const int &closure, StmtType type, int stmtNumber) -> bool {
        switch (stmtNumber) {
          case 1:
            return type == StmtType::Assign;
          case 2:
            return type == StmtType::Assign;
          case 3:
            return type == StmtType::Print;
          case 4:
            return type == StmtType::Assign;
          default:
            return false;
        }
      };
  auto cfg = TestCFGProvider::getLinearCFG();
  CFGNextQuerier<int, typePredicate> querier(&cfg, 0);

  auto result = queryNext(&querier,
                          StmtRef{StmtType::Assign, 0},
                          StmtRef{StmtType::None, 2});
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryNext(&querier,
                     StmtRef{StmtType::Print, 0},
                     StmtRef{StmtType::None, 2});
  REQUIRE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{});

  result = queryNext(&querier,
                     StmtRef{StmtType::None, 2},
                     StmtRef{StmtType::Print, 0});
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryNext(&querier,
                     StmtRef{StmtType::None, 2},
                     StmtRef{StmtType::Assign, 0});
  REQUIRE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{});

  result = StmtTransitiveResult();
  queryNext(&querier, &result,
            StmtRef{StmtType::Assign, 0},
            StmtRef{StmtType::Assign, 0});
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2}
  });

  result = StmtTransitiveResult();
  queryNext(&querier,
            &result,
            StmtRef{StmtType::Assign, 0},
            StmtRef{StmtType::Print, 0});
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {2, 3}
  });
}

TEST_CASE("Next While-Cycle Path") {
  auto cfg = TestCFGProvider::getAffectsWhileCFG();
  CFGTestNextQuerier querier(&cfg, 0);

  StmtTransitiveResult result;
  queryNext(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);

  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {2, 3},
      {3, 2},
      {2, 4}
  });
}
