#include "catch.hpp"
#include "TestCFGProvider.h"
#include "qps/executor/QueryCache.h"
#include "CFGTestModifiesUsesProvider.h"
#include "qps/cfg/cfg_querier/CFGAffectsQuerier.h"

typedef CFGAffectsQuerier<CFGTestModifiesUsesProvider>
    CFGTestAffectsQuerier;

TEST_CASE("Test Affects Cache Promotion and Contents") {
  auto cfg = TestCFGProvider::getLinearCFG();
  QueryCache cache;
  CacheTable *cacheTable = cache.getAffectsCache();
  StmtTransitiveResult result;
  auto pkbProvider =
      CFGTestModifiesUsesProvider({{"x"}, {"y"}, {"z"}, {"w"}},
                                  {{}, {}, {"x"}, {"y"}},
                                  &cache);
  CFGTestAffectsQuerier querier(&cfg, pkbProvider);

  REQUIRE(cacheTable->queryFull(1, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(2, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(3, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(4, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 1) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 4) == nullptr);

  result = StmtTransitiveResult{};
  querier.queryBool(&result, 1, 2);
  REQUIRE(cacheTable->queryFull(1, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryPartial(1, 2) == nullptr);
  REQUIRE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryBool(&result, 1, 3);
  REQUIRE(cacheTable->queryFull(1, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) == nullptr);
  REQUIRE(cacheTable->queryPartial(1, 3) != nullptr);
  REQUIRE_FALSE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryFrom(&result, 1, StmtType::Wildcard);
  REQUIRE(cacheTable->queryFull(1, 0) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) == nullptr);
  REQUIRE_FALSE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryFrom(&result, 1, StmtType::Assign);
  REQUIRE(cacheTable->queryFull(1, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 1) == nullptr);
  REQUIRE(cacheTable->queryPartial(1, 3));
  REQUIRE_FALSE(cacheTable->queryPartial(3, 1));
  REQUIRE_FALSE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryTo(&result, StmtType::Wildcard, 3);
  REQUIRE(cacheTable->queryFull(1, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 1) == nullptr);
  REQUIRE_FALSE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryTo(&result, StmtType::Assign, 3);
  REQUIRE(cacheTable->queryFull(1, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 1) == nullptr);
  REQUIRE_FALSE(result.empty());

  result = StmtTransitiveResult{};
  querier.queryAll(&result, StmtType::Assign, StmtType::Assign);
  REQUIRE(cacheTable->queryFull(1, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(2, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(3, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(4, 0) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 1) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 2) == nullptr);
  REQUIRE(cacheTable->queryFull(0, 3) != nullptr);
  REQUIRE(cacheTable->queryFull(0, 4) == nullptr);
  REQUIRE_FALSE(result.empty());
}
