#include "CFGTestModifiesUsesProvider.h"
#include "TestCFGProvider.h"
#include "catch.hpp"
#include "qps/cfg/cfg_querier/CFGAffectsTQuerier.h"

typedef CFGAffectsQuerier<CFGTestModifiesUsesProvider> ConcreteAffectsQuerier;
typedef CFGAffectsTQuerier<ConcreteAffectsQuerier> CFGTestAffectsTQuerier;

template <typename T>
StmtTransitiveResult queryAffectsT(CFGAffectsTQuerier<T>* querier, int left,
                                   int right) {
  StmtTransitiveResult ret;
  querier->queryArgs(StmtRef{StmtType::None, left},
                     StmtRef{StmtType::None, right}, &ret);
  return ret;
}

template <typename T>
void queryAffectsT(CFGAffectsTQuerier<T>* querier, StmtTransitiveResult* output,
                   int left, int right) {
  querier->queryArgs(StmtRef{StmtType::None, left},
                     StmtRef{StmtType::None, right}, output);
}

template <typename T>
StmtTransitiveResult queryAffectsT(CFGAffectsTQuerier<T>* querier, StmtRef left,
                                   StmtRef right) {
  return querier->queryArgs(left, right);
}

template <typename T>
void queryAffectsT(CFGAffectsTQuerier<T>* querier, StmtTransitiveResult* output,
                   StmtRef left, StmtRef right) {
  querier->queryArgs(left, right, output);
}

template <typename T>
void assertQueryAffectsTEmpty(CFGAffectsTQuerier<T>* querier, int left,
                              unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE(queryAffectsT(querier, left, *it).empty());
  }
}

template <typename T>
void assertQueryAffectsTNotEmpty(CFGAffectsTQuerier<T>* querier, int left,
                                 unordered_set<int> rights) {
  for (auto it = rights.begin(); it != rights.end(); it++) {
    REQUIRE_FALSE(queryAffectsT(querier, left, *it).empty());
  }
}

QueryCache globalCache;

CFGTestModifiesUsesProvider transitiveLinearProvider(
    {{"a"}, {"b"}, {"c"}, {"d"}}, {{}, {"a"}, {"b"}, {"e"}}, &globalCache);

CFGTestModifiesUsesProvider whileProvider({{}, {"a"}, {"b"}, {"c"}},
                                          {{}, {"b"}, {"c"}, {"a"}},
                                          {{1, StmtType::While}}, &globalCache);

CFGTestModifiesUsesProvider ifNoneProvider({{"x"}, {}, {"y"}, {"y"}, {"z"}},
                                           {{}, {"x"}, {"x"}, {"x"}, {"x"}},
                                           {{2, StmtType::If}}, &globalCache);

CFGTestModifiesUsesProvider ifThenProvider({{"x"}, {}, {"y"}, {"x"}, {"z"}},
                                           {{}, {"x"}, {"x"}, {}, {"y"}},
                                           {{2, StmtType::If}}, &globalCache);

CFGTestModifiesUsesProvider ifElseProvider({{"x"}, {}, {"x"}, {"y"}, {"z"}},
                                           {{}, {"x"}, {}, {"x"}, {"y"}},
                                           {{2, StmtType::If}}, &globalCache);
//
// CFGTestModifiesUsesProvider ifBothProvider(
//
//    new QueryCache()
//);

TEST_CASE("AffectsT Linear (Const, Const)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"a"}, {"b"}, {"c"}, {"d"}}, {{}, {"a"}, {"b"}, {"e"}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);
  assertQueryAffectsTNotEmpty(&querier, 1, {2, 3});
  assertQueryAffectsTNotEmpty(&querier, 2, {3});
  assertQueryAffectsTEmpty(&querier, 1, {4});
  assertQueryAffectsTEmpty(&querier, 2, {2, 4});
  assertQueryAffectsTEmpty(&querier, 3, {1, 2, 4});
  assertQueryAffectsTEmpty(&querier, 4, {1, 2, 3});
}

TEST_CASE("AffectsT While (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{}, {"a"}, {"b"}, {"c"}}, {{}, {"b"}, {"c"}, {"a"}},
      {{1, StmtType::While}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 2, {3, 4, 2});
  assertQueryAffectsTNotEmpty(&querier, 3, {2, 3, 4});
  assertQueryAffectsTNotEmpty(&querier, 4, {2, 3, 4});
}

TEST_CASE("AffectsT If None path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"x"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4, 5});
}

TEST_CASE("AffectsT If Then path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"x"}, {"z"}}, {{}, {"x"}, {"x"}, {}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);
  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});
}

TEST_CASE("AffectsT If Else path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfElseCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"x"}, {"y"}, {"z"}}, {{}, {"x"}, {}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);
  assertQueryAffectsTNotEmpty(&querier, 1, {4, 5});
  assertQueryAffectsTNotEmpty(&querier, 4, {5});
}

TEST_CASE("AffectsT If Both path (Const, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});
  assertQueryAffectsTNotEmpty(&querier, 4, {5});
}

TEST_CASE("AffectsT Linear (Const, _)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"a"}, {"b"}, {"c"}, {"d"}}, {{}, {"a"}, {"b"}, {"e"}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);
  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{2, 3});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{3});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT While (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{}, {"a"}, {"b"}, {"c"}}, {{}, {"b"}, {"c"}, {"a"}},
      {{1, StmtType::While}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{2, 3, 4});
}

TEST_CASE("AffectsT If None path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"x"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{3, 4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT If Then path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"x"}, {"z"}}, {{}, {"x"}, {"x"}, {}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{3, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT If Else path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfElseCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"x"}, {"y"}, {"z"}}, {{}, {"x"}, {}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT If Both path (Const, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 1, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{3, 4, 5});

  result = queryAffectsT(&querier, 2, 0);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 3, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 4, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getRightVals() == unordered_set<StmtValue>{5});

  result = queryAffectsT(&querier, 5, 0);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT Linear (_, Const)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"a"}, {"b"}, {"c"}, {"d"}}, {{}, {"a"}, {"b"}, {"e"}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1, 2});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE(result.empty());
}

TEST_CASE("AffectsT While (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{}, {"a"}, {"b"}, {"c"}}, {{}, {"b"}, {"c"}, {"a"}},
      {{1, StmtType::While}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{2, 3, 4});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{2, 3, 4});
}

TEST_CASE("AffectsT If None path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"x"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 1, {3, 4});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});
}

TEST_CASE("AffectsT If Then path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"x"}, {"z"}}, {{}, {"x"}, {"x"}, {}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  assertQueryAffectsTNotEmpty(&querier, 1, {3, 5});
  assertQueryAffectsTNotEmpty(&querier, 3, {5});

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1, 3});
}

TEST_CASE("AffectsT If Else path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfElseCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"x"}, {"y"}, {"z"}}, {{}, {"x"}, {}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1, 4});
}

TEST_CASE("AffectsT If Both path (_, Const)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  StmtTransitiveResult result;
  result = queryAffectsT(&querier, 0, 1);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 2);
  REQUIRE(result.empty());

  result = queryAffectsT(&querier, 0, 3);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 4);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1});

  result = queryAffectsT(&querier, 0, 5);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getLeftVals() == unordered_set<StmtValue>{1, 3, 4});
}

TEST_CASE("AffectsT Linear (_, _)") {
  auto cfg = TestCFGProvider::getLinearTransitiveCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"a"}, {"b"}, {"c"}, {"d"}}, {{}, {"a"}, {"b"}, {"e"}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() == pair_set<StmtValue, StmtValue>{
                                      {1, 2},
                                      {1, 3},
                                      {2, 3},
                                  });
}

TEST_CASE("AffectsT While (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTWhileCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{}, {"a"}, {"b"}, {"c"}}, {{}, {"b"}, {"c"}, {"a"}},
      {{1, StmtType::While}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() == pair_set<StmtValue, StmtValue>{
                                      {2, 2},
                                      {2, 3},
                                      {2, 4},
                                      {3, 2},
                                      {3, 3},
                                      {3, 4},
                                      {4, 2},
                                      {4, 3},
                                      {4, 4},
                                  });
}

TEST_CASE("AffectsT If None path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfNoneCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"x"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() ==
          pair_set<StmtValue, StmtValue>{{1, 3}, {1, 4}, {1, 5}});
}

TEST_CASE("AffectsT If Then path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfThenCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"x"}, {"z"}}, {{}, {"x"}, {"x"}, {}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() ==
          pair_set<StmtValue, StmtValue>{{1, 3}, {1, 5}, {3, 5}});
}

TEST_CASE("AffectsT If Else path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfElseCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"x"}, {"y"}, {"z"}}, {{}, {"x"}, {}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() ==
          pair_set<StmtValue, StmtValue>{{1, 4}, {1, 5}, {4, 5}});
}

TEST_CASE("AffectsT If Both path (_, _)") {
  auto cfg = TestCFGProvider::getAffectsTIfBothCFG();
  QueryCache cache;
  auto pkbProvider = CFGTestModifiesUsesProvider(
      {{"x"}, {}, {"y"}, {"y"}, {"z"}}, {{}, {"x"}, {"x"}, {"x"}, {"y"}},
      {{2, StmtType::If}}, &cache);
  ConcreteAffectsQuerier affectsQ(&cfg, pkbProvider);
  CFGTestAffectsTQuerier querier(&cfg, affectsQ);

  auto result = StmtTransitiveResult();
  queryAffectsT(&querier, &result, 0, 0);
  REQUIRE_FALSE(result.empty());
  REQUIRE(result.getPairVals() == pair_set<StmtValue, StmtValue>{
                                      {1, 3}, {1, 4}, {1, 5}, {3, 5}, {4, 5}});
}
