#include "catch.hpp"

#include "TestCFGProvider.h"
#include "qps/cfg/cfg_querier/CFGAffectsTQuerier.h"
#include "CFGTestModifiesUsesProvider.h"

typedef CFGAffectsTQuerier<CFGTestModifiesUsesProvider,
                           CFGTestModifiesUsesProvider::typePredicate,
                           CFGTestModifiesUsesProvider::getModifies,
                           CFGTestModifiesUsesProvider::getUses,
                           CFGTestModifiesUsesProvider::getCount,
                           CFGTestModifiesUsesProvider::getSymbolId> CFGTestAffectsTQuerier;

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG, CountGetter<T> CG, SymbolIdGetter<T> SG>
StmtTransitiveResult queryAffectsT(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                                   int left, int right) {
  return querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right}
  );
}

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG,
    CountGetter<T> CG, SymbolIdGetter<T> SG>
void queryAffectsT(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                   StmtTransitiveResult* output,
                   int left, int right) {
  querier->queryArgs(
      StmtRef{StmtType::None, left},
      StmtRef{StmtType::None, right},
      output
  );
}

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG,
    CountGetter<T> CG, SymbolIdGetter<T> SG>
StmtTransitiveResult queryAffectsT(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                                   StmtRef left,
                                   StmtRef right) {
  return querier->queryArgs(left, right);
}

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG,
    CountGetter<T> CG, SymbolIdGetter<T> SG>
void queryAffectsT(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                   StmtTransitiveResult* output,
                   StmtRef left,
                   StmtRef right) {
  querier->queryArgs(left, right, output);
}

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG,
    CountGetter<T> CG, SymbolIdGetter<T> SG>
void assertQueryAffectsTEmpty(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                              int left,
                              unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE(queryAffectsT(querier, left, *it).isEmpty);
  }
}

template <typename T, StmtTypePredicate<T> U, ModifiesGetter<T> MG, UsesGetter<T> UG,
    CountGetter<T> CG, SymbolIdGetter<T> SG>
void assertQueryAffectsTNotEmpty(CFGAffectsTQuerier<T, U, MG, UG, CG, SG>* querier,
                                 int left,
                                 unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE_FALSE(queryAffectsT(querier, left, *it).isEmpty);
  }
}

CFGTestModifiesUsesProvider transitiveLinearProvider(
    { "a", "b", "c", "d"},
    {{}, {"a"}, {"b"}, {"e"}},
    5,
    {{"a", 0}, {"b", 1}, {"c", 2}, {"d", 3}, {"e" , 4}},
    {}
);

CFGTestModifiesUsesProvider whileProvider(
    { "", "a", "b", "c"},
    {{}, {"b"}, {"c"}, {"a"}},
    4,
    {{"a", 0}, {"b", 1}, {"c", 2}, {"x", 3}},
    {{1, StmtType::While}}
);

CFGTestModifiesUsesProvider ifNoneProvider(
    { "x", "", "y", "y", "z"},
    {{}, {"x"}, {"x"}, {"x"}, {"x"}},
    3,
    {{"x", 0}, {"y", 1}, {"z", 2}},
    {{2, StmtType::If}}
);

CFGTestModifiesUsesProvider ifThenProvider(
    { "x", "", "y", "x", "z"},
    {{}, {"x"}, {"x"}, {}, {"y"}},
    3,
    {{"x", 0}, {"y", 1}, {"z", 2}},
    {{2, StmtType::If}}
);

CFGTestModifiesUsesProvider ifElseProvider(
    { "x", "", "x", "y", "z"},
    {{}, {"x"}, {}, {"x"}, {"y"}},
    3,
    {{"x", 0}, {"y", 1}, {"z", 2}},
    {{2, StmtType::If}}
);

CFGTestModifiesUsesProvider ifBothProvider(
    { "x", "", "y", "y", "z"},
    {{}, {"x"}, {"x"}, {"x"}, {"y"}},
    3,
    {{"x", 0}, {"y", 1}, {"z", 2}},
    {{2, StmtType::If}}
);

TEST_CASE("AffectsT Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {2, 3});
  assertQueryAffectsTNotEmpty(&querier, 2, {3});
  assertQueryAffectsTEmpty(&querier, 1, {4});
  assertQueryAffectsTEmpty(&querier, 2, {2,4});
  assertQueryAffectsTEmpty(&querier, 3, {1,2,4});
  assertQueryAffectsTEmpty(&querier, 4, {1,2,3});
}

TEST_CASE("AffectsT While (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);
  assertQueryAffectsTNotEmpty(&querier, 2, {3, 4, 2});
  assertQueryAffectsTNotEmpty(&querier, 3, {2, 3, 4});
  assertQueryAffectsTNotEmpty(&querier, 4, {2, 3, 4});
}

TEST_CASE("AffectsT If None path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifNoneProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4, 5});
}

TEST_CASE("AffectsT If Then path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifThenProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});
}

TEST_CASE("AffectsT If Else path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifElseProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {4, 5});
  assertQueryAffectsTNotEmpty(&querier, 4, {5});
}

TEST_CASE("AffectsT If Both path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifBothProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});
  assertQueryAffectsTNotEmpty(&querier, 4, {5});
}

TEST_CASE("AffectsT Linear (Const, _)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);

  StmtTransitiveResult  result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.isEmpty);

}

TEST_CASE("AffectsT While (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);
  assertQueryAffectsTNotEmpty(&querier, 2, {3, 4, 2});
  assertQueryAffectsTNotEmpty(&querier, 3, {2, 3, 4});
  assertQueryAffectsTNotEmpty(&querier, 4, {2, 3, 4});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3, 4});
}

TEST_CASE("AffectsT If None path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifNoneProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT If Then path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifThenProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT If Else path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifElseProvider);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.isEmpty);


  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT If Both path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifBothProvider);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});


  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT Linear (_, Const)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);

  StmtTransitiveResult  result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT While (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{2, 3, 4});
}

TEST_CASE("AffectsT If None path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifNoneProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});
}

TEST_CASE("AffectsT If Then path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifThenProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 3});
}

TEST_CASE("AffectsT If Else path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifElseProvider);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 4});
}

TEST_CASE("AffectsT If Both path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifBothProvider);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 3, 4});
}

TEST_CASE("AffectsT Linear (_, _)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {1, 3},
      {2, 3},
  });
}

TEST_CASE("AffectsT While (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {2, 2}, {2, 3}, {2, 4},
      {3, 2}, {3, 3}, {3, 4},
      {4, 2}, {4, 3}, {4, 4},
  });
}

TEST_CASE("AffectsT If None path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifNoneProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 3},
      {1, 4},
      {1, 5}
  });
}

TEST_CASE("AffectsT If Then path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifThenProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 3},
      {1, 5},
      {3, 5}
  });
}

TEST_CASE("AffectsT If Else path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifElseProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 4},
      {1, 5},
      {4, 5}
  });
}

TEST_CASE("AffectsT If Both path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifBothProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 3},
      {1, 4},
      {1, 5},
      {3, 5},
      {4, 5}
  });
}
