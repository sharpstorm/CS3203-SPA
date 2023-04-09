#include "catch.hpp"

#include "TestCFGProvider.h"
#include "qps/cfg/cfg_querier/CFGAffectsQuerier.h"
#include "CFGTestModifiesUsesProvider.h"

typedef CFGAffectsQuerier<CFGTestModifiesUsesProvider>
    CFGTestAffectsQuerier;

template<typename T>
StmtTransitiveResult queryAffects(CFGAffectsQuerier<T> *querier,
                                  int left, int right) {
  return querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right}
  );
}

template<typename T>
void queryAffects(CFGAffectsQuerier<T> *querier,
                  StmtTransitiveResult *output,
                  int left, int right) {
  querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right},
      output
  );
}

template<typename T>
StmtTransitiveResult queryAffects(CFGAffectsQuerier<T> *querier,
                                  StmtRef left,
                                  StmtRef right) {
  return querier->queryArgs(left, right);
}

template<typename T>
void queryAffects(CFGAffectsQuerier<T> *querier,
                  StmtTransitiveResult *output,
                  StmtRef left,
                  StmtRef right) {
  querier->queryArgs(left, right, output);
}

template<typename T>
void assertQueryAffectsEmpty(CFGAffectsQuerier<T> *querier,
                             int left,
                             unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE(queryAffects(querier, left, *it).isEmpty);
  }
}

template<typename T>
void assertQueryAffectsNotEmpty(CFGAffectsQuerier<T> *querier,
                                int left,
                                unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE_FALSE(queryAffects(querier, left, *it).isEmpty);
  }
}

TEST_CASE("Affects Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  assertQueryAffectsNotEmpty(&querier, 1, {3});
  assertQueryAffectsNotEmpty(&querier, 2, {4});

  assertQueryAffectsEmpty(&querier, 1, {1, 2, 4});
  assertQueryAffectsEmpty(&querier, 2, {1, 2, 3});
  assertQueryAffectsEmpty(&querier, 3, {1, 2, 3, 4});
  assertQueryAffectsEmpty(&querier, 4, {1, 2, 3, 4});
}

TEST_CASE("Affects Linear (Const, _)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  &cache);

  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryAffects(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{4});

  result = queryAffects(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 4, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 0 |  x = 1;
 * 1 |  y = 2;
 * 2 |  z = x;
 * 3 |  w = y;
 */
TEST_CASE("Affects Linear (_, Const)") {
  auto cfg = TestCFGProvider::getLinearCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  &cache);

  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2});
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    x = x;
 *   |  } else {
 * 4 |    x = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If No Through Path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryAffects(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffects(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffects(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    y = x;
 *   |  } else {
 * 4 |    x = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Then Through Path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"y"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 5});

  result = queryAffects(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffects(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    x = x;
 *   |  } else {
 * 4 |    y = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Else Through Path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"y"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 5});

  result = queryAffects(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffects(&querier, 4, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    z = x;
 *   |  } else {
 * 4 |    y = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Both Through Path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"z"}, {"y"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 5});

  result = queryAffects(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 4, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 1 |  x = 1;
 * 2 |  while (x != 1) {
 * 3 |    x = x;
 *   |  }
 * 4 |  x = x;
 */
TEST_CASE("Affects While (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsWhileCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::While}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4});

  result = queryAffects(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4});

  result = queryAffects(&querier, 4, 0);
  REQUIRE(result.isEmpty);
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    x = x;
 *   |  } else {
 * 4 |    x = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If No Through Path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{3, 4});
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    y = x;
 *   |  } else {
 * 4 |    x = x;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Then Through Path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"y"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 4});
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    x = x;
 *   |  } else {
 * 4 |    y = x;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Else Through Path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"y"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 3});
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    z = x;
 *   |  } else {
 * 4 |    y = x;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Both Through Path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"z"}, {"y"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});
}

/*
 * 1 |  x = 1;
 * 2 |  while (x != 1) {
 * 3 |    x = x;
 *   |  }
 * 4 |  x = x;
 */
TEST_CASE("Affects While (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsWhileCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::While}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 3});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 3});
}

/*
 * 1 |  x = 1;
 * 2 |  while (x != 1) {
 * 3 |    y = x;
 *   |  }
 * 4 |  x = x;
 */
TEST_CASE("Affects While No Mod in Loop (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsWhileCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"y"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::While}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = queryAffects(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffects(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffects(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});
}

/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    x = x;
 *   |  } else {
 * 4 |    x = 2;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If No Through Path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"x"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = StmtTransitiveResult();
  queryAffects(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue>{
      {1, 3},
      {3, 5},
      {4, 5}
  });
}


/*
 * 1 |  x = 1;
 * 2 |  if (x != 1) then {
 * 3 |    y = x;
 *   |  } else {
 * 4 |    x = x;
 *   |  }
 * 5 |  x = x;
 */
TEST_CASE("Affects If Then Through Path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsIfCFG();
  QueryCache cache;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {}, {"y"}, {"x"}, {"x"}},
                                  {{}, {"x"}, {"x"}, {"x"}, {"x"}},
                                  {{2, StmtType::If}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  auto result = StmtTransitiveResult();
  queryAffects(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue>{
      {1, 3},
      {1, 4},
      {1, 5},
      {4, 5}
  });
}
