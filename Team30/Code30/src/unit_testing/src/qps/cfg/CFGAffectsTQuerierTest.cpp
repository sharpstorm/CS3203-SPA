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

CFGTestModifiesUsesProvider ifProvider(
    { "x", "", "y", "x", "z"},
    {{}, {"x"}, {"x"}, {}, {"y"}},
    3,
    {{"x", 0}, {"y", 1}, {"z", 2}},
    {{2, StmtType::If}}
);

TEST_CASE("AffectsT Linear (Const, Const)") {
  /*
   * 0 | a = 1;
   * 1 | b = a;
   * 2 | c = b;
   * 3 | d = e;
   */
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {2, 3});
  assertQueryAffectsTNotEmpty(&querier, 2, {3});
  assertQueryAffectsTEmpty(&querier, 1, {4});

}

TEST_CASE("AffectsT If (Const, Const)") {
  /*
  * 0 |  x = 1;
  * 1 |  if (x != 1) then {
  * 2 |    y = x;
  *   |  } else {
  * 3 |    x = 2;
  *   |  }
  * 4 |  z = y;
  */
  auto cfg = TestCFGProvider::getAffectsTIfCFG();
  CFGTestAffectsTQuerier querier(&cfg, ifProvider);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});
}

TEST_CASE("AffectsT While (Const, Const)") {
  /*
  * 0 | while (x < 0) {
  * 1 |  a = b;
  * 2 |  b = c;
  * 3 |  c = a;
  * }
  * */
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);
  assertQueryAffectsTNotEmpty(&querier, 2, {3, 4});
  assertQueryAffectsTNotEmpty(&querier, 3, {2, 4});
  assertQueryAffectsTNotEmpty(&querier, 4, {2, 3});
}


TEST_CASE("AffectsT Linear (Const, _)") {
/*
  * 0 | a = 1;
  * 1 | b = a;
  * 2 | c = b;
  * 3 | d = e;
  * */


  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  CFGTestAffectsTQuerier querier(&cfg, transitiveLinearProvider);


  StmtTransitiveResult  result;
//  result = queryAffectsT(&querier, 1, 0);
//  REQUIRE_FALSE(result.isEmpty);
//  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3});

//  result = queryAffectsT(&querier, 2, 0);
//  REQUIRE_FALSE(result.isEmpty);
//  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.isEmpty);
}

TEST_CASE("AffectsT While (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  CFGTestAffectsTQuerier querier(&cfg, whileProvider);

  auto result = queryAffectsT(&querier, 1, 0);
  REQUIRE(result.isEmpty);

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{3, 4});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 4});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.secondArgVals == unordered_set<StmtValue>{2, 3});
}

TEST_CASE("AffectsT Linear (_, Const)") {
/*
  * 0 | a = 1;
  * 1 | b = a;
  * 2 | c = b;
  * 3 | d = e;
  * */
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, CFG_END_NODE);

  auto pkbProvider =
      CFGTestModifiesUsesProvider({ "a", "b", "c", "d"},
                                  { {}, {"a"}, {"b"}, {"e"}},
                                  4,
                                  {{"a", 0}, {"b", 1}, {"c", 2}, {"d", 3}, {"e", 4}},
                                  {});

  CFGTestAffectsTQuerier querier(&cfg, pkbProvider);

  auto result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.firstArgVals == unordered_set<StmtValue>{1, 2});
}

TEST_CASE("AffectsT Linear (_, _)") {
/*
  * 0 | a = 1;
  * 1 | b = a;
  * 2 | c = b;
  * 3 | d = e;
  * */
  CFG cfg(1);
  cfg.addLink(0, 1);
  cfg.addLink(1, 2);
  cfg.addLink(2, 3);
  cfg.addLink(3, CFG_END_NODE);

  auto pkbProvider =
      CFGTestModifiesUsesProvider({ "a", "b", "c", "d"},
                                  { {}, {"a"}, {"b"}, {"e"}},
                                  5,
                                  {{"a", 0}, {"b", 1}, {"c", 2}, {"d", 3}, {"e", 4}},
                                  {});

  CFGTestAffectsTQuerier querier(&cfg, pkbProvider);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.isEmpty);
  REQUIRE(result.pairVals == pair_set<StmtValue, StmtValue> {
      {1, 2},
      {2, 3},
  });
}

