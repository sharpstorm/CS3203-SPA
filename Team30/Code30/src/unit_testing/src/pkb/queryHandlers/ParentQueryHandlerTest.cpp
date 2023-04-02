#include <memory>
#include <unordered_set>
#include <utility>

#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/queryHandlers/ParentTQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::unique_ptr;
using std::unordered_set;

static std::unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->insert(2, StmtType::Assign);
  provider->insert(3, StmtType::Assign);
  provider->insert(4, StmtType::Read);
  provider->insert(5, StmtType::Read);
  provider->insert(11, StmtType::If);
  provider->insert(14, StmtType::If);
  provider->insert(12, StmtType::While);
  provider->insert(16, StmtType::While);

  provider->allStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  return provider;
}

struct parentTest {
  shared_ptr<ParentTable> table = make_shared<ParentTable>();
  shared_ptr<ParentRevTable> reverseTable = make_shared<ParentRevTable>();
  unique_ptr<ParentStorage> store =
      make_unique<ParentStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      setUpStructureMappingProvider();
  unique_ptr<StmtPredicateFactory> factory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<PkbStmtStmtQueryInvoker> invoker =
      make_unique<PkbStmtStmtQueryInvoker>(structureProvider.get(),
                                           factory.get());
  ParentQueryHandler handler = ParentQueryHandler(invoker.get(), store.get());

  QueryResultPtr<StmtValue, StmtValue> query(StmtRef leftArg,
                                             StmtRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }
};

/* Parent */

TEST_CASE("ParentQueryHandler parent(stmtNum,stmtNum)") {
  auto test = parentTest();

  test.table->insert(1, 2);
  test.table->insert(2, 4);

  REQUIRE(test.query({StmtType::None, 1}, {StmtType::None, 2}).get()->isEmpty ==
          false);
  REQUIRE(test.query({StmtType::None, 2}, {StmtType::None, 1}).get()->isEmpty ==
          true);
  REQUIRE(test.query({StmtType::None, 4}, {StmtType::None, 4}).get()->isEmpty ==
          true);
  REQUIRE(test.query({StmtType::None, 1}, {StmtType::None, 1}).get()->isEmpty ==
          true);
}

TEST_CASE("ParentQueryHandler parent(stmtNum,stmtType)") {
  auto test = parentTest();

  test.table->insert(1, 2);
  test.table->insert(2, 3);
  test.table->insert(3, 5);

  auto result1 = *test.query({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = *test.query({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parent(stmtType, stmtNum)") {
  auto test = parentTest();

  test.reverseTable->insert(12, 11);

  auto result1 = *test.query({StmtType::If, 0}, {StmtType::None, 12});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 12}}));

  auto result2 = *test.query({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parent(stmtType, stmtType)") {
  auto test = parentTest();

  test.table->insert(10, 11);
  test.table->insert(11, 12);
  test.table->insert(12, 16);

  auto result1 = *test.query({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 12}}));
}

TEST_CASE("ParentQueryHandler parent(_, stmtType)") {
  auto test = parentTest();

  test.store->insert(10, 11);
  test.store->insert(11, 12);
  test.store->insert(12, 16);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.secondArgVals == unordered_set<int>({11, 12, 16}));
}

TEST_CASE("ParentQueryHandler parent(_, stmtNum)") {
  auto test = parentTest();

  test.store->insert(10, 11);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::None, 11});
  REQUIRE(result1.isEmpty == false);
}

TEST_CASE("ParentQueryHandler parent(_, _)") {
  auto test = parentTest();

  test.store->insert(10, 11);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.isEmpty == false);
}

TEST_CASE("ParentQueryHandler parent(_, _) no results") {
  auto test = parentTest();

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.isEmpty == true);
}

struct parentTTest {
  shared_ptr<ParentTTable> table = make_shared<ParentTTable>();
  shared_ptr<ParentTRevTable> reverseTable = make_shared<ParentTRevTable>();
  shared_ptr<FollowsTable> followsTable = make_shared<FollowsTable>();
  unique_ptr<ParentTStorage> storeT = make_unique<ParentTStorage>(
      table.get(), reverseTable.get(), followsTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      make_unique<StructureMappingProviderStub>();
  unique_ptr<StmtPredicateFactory> factory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  ParentTQueryHandler handlerT =
      ParentTQueryHandler(storeT.get(), structureProvider.get(), factory.get());

  QueryResultPtr<StmtValue, StmtValue> queryT(StmtRef leftArg,
                                              StmtRef rightArg) {
    return handlerT.query(&leftArg, &rightArg);
  }

  parentTTest() {
    // 1(->2(->3(->4,5|6)),7),8
    storeT->insert(1, 7);
    storeT->insert(1, 6);
    storeT->insert(1, 5);
    storeT->insert(1, 3);
    storeT->insert(2, 3);
    storeT->insert(2, 5);
    storeT->insert(2, 6);
    storeT->insert(3, 5);
    storeT->insert(3, 6);
    followsTable->insert(1, 8);
    followsTable->insert(2, 7);
    followsTable->insert(4, 5);
    structureProvider->insert(1, StmtType::While);
    structureProvider->insert(2, StmtType::While);
    structureProvider->insert(3, StmtType::If);
    structureProvider->insert(4, StmtType::Assign);
    structureProvider->insert(5, StmtType::Read);
    structureProvider->insert(6, StmtType::Assign);
    structureProvider->insert(7, StmtType::Assign);
    structureProvider->insert(8, StmtType::Assign);
    structureProvider->allStmts = {1, 2, 3, 4, 5, 6, 7, 8};
  }
};

/* ParentStar */
TEST_CASE("ParentQueryHandler parentStar(stmtNum,stmtNum)") {
  auto test = parentTTest();
  // level 2
  unordered_set<int> level2({2, 7});
  for (auto i : level2) {
    SECTION("Test with Parent: 1, Child: " + std::to_string(i)) {
      auto result = (*test.queryT({StmtType::None, 1}, {StmtType::None, i}));
      REQUIRE(result.isEmpty == false);
    }
  }

  // level 3
  unordered_set<int> parentsL3({1, 2});
  for (auto i : parentsL3) {
    SECTION("Test with Parent: " + std::to_string(i) + ", Child: 3") {
      auto result = (*test.queryT({StmtType::None, i}, {StmtType::None, 3}));
      REQUIRE(result.isEmpty == false);
    }
  }

  // level 4
  unordered_set<int> parentsL4({1, 2, 3});
  unordered_set<int> level4({4, 5, 6});
  for (auto p : parentsL4) {
    SECTION("Test with Parent: " + std::to_string(p)) {
      for (auto c : level4) {
        SECTION("Test with Child: " + std::to_string(c)) {
          auto result =
              (*test.queryT({StmtType::None, p}, {StmtType::None, c}));
          REQUIRE(result.isEmpty == false);
        }
      }
    }
  }

  auto result1 = *test.queryT({StmtType::None, 1}, {StmtType::None, 1});
  REQUIRE(result1.isEmpty == true);
  auto result2 = *test.queryT({StmtType::None, 1}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
  auto result3 = *test.queryT({StmtType::None, 2}, {StmtType::None, 1});
  REQUIRE(result3.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parentStar(stmtNum,stmtType)") {
  auto test = parentTTest();

  auto result1 = *test.queryT({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.secondArgVals == unordered_set<int>({4, 6, 7}));
  auto result2 = *test.queryT({StmtType::None, 2}, {StmtType::Assign, 0});
  REQUIRE(result2.secondArgVals == unordered_set<int>({4, 6}));
  auto result3 = *test.queryT({StmtType::None, 1}, {StmtType::While, 0});
  REQUIRE(result3.secondArgVals == unordered_set<int>({2}));
  auto result4 = *test.queryT({StmtType::None, 1}, {StmtType::None, 0});
  REQUIRE(result4.secondArgVals == unordered_set<int>({2, 3, 4, 5, 6, 7}));
  auto result5 = *test.queryT({StmtType::None, 2}, {StmtType::None, 0});
  REQUIRE(result5.secondArgVals == unordered_set<int>({3, 4, 5, 6}));

  auto result6 = *test.queryT({StmtType::None, 4}, {StmtType::None, 0});
  REQUIRE(result6.secondArgVals == unordered_set<int>({}));
  auto result7 = *test.queryT({StmtType::None, 6}, {StmtType::None, 0});
  REQUIRE(result7.secondArgVals == unordered_set<int>({}));
  auto result8 = *test.queryT({StmtType::None, 1}, {StmtType::Print, 0});
  REQUIRE(result7.secondArgVals == unordered_set<int>({}));
}

TEST_CASE("ParentQueryHandler parentStar(stmtType,stmtNum)") {
  auto test = parentTTest();

  // level 1
  unordered_set<int> level1({1, 8});
  for (auto i : level1) {
    SECTION("Test with Child: " + std::to_string(i)) {
      auto result1 = (*test.queryT({StmtType::None, 0}, {StmtType::None, i}));
      REQUIRE(result1.firstArgVals == unordered_set<int>({}));
    }
  }
  // level 2
  unordered_set<int> level2({2, 7});
  for (auto i : level2) {
    SECTION("Test with Child: " + std::to_string(i)) {
      auto result1 = (*test.queryT({StmtType::While, 0}, {StmtType::None, i}));
      REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
      auto result2 = (*test.queryT({StmtType::If, 0}, {StmtType::None, i}));
      REQUIRE(result2.firstArgVals == unordered_set<int>({}));
      auto result3 = (*test.queryT({StmtType::None, 0}, {StmtType::None, i}));
      REQUIRE(result3.firstArgVals == unordered_set<int>({1}));
    }
  }
  // level 3
  auto result1 = (*test.queryT({StmtType::While, 0}, {StmtType::None, 3}));
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2}));
  auto result2 = (*test.queryT({StmtType::If, 0}, {StmtType::None, 3}));
  REQUIRE(result2.firstArgVals == unordered_set<int>({}));
  auto result3 = (*test.queryT({StmtType::None, 0}, {StmtType::None, 3}));
  REQUIRE(result3.firstArgVals == unordered_set<int>({1, 2}));

  // level 4
  unordered_set<int> level4({4, 5, 6});
  for (auto i : level4) {
    SECTION("Test with Child: " + std::to_string(i)) {
      auto result1 = (*test.queryT({StmtType::While, 0}, {StmtType::None, i}));
      REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2}));
      auto result2 = (*test.queryT({StmtType::If, 0}, {StmtType::None, i}));
      REQUIRE(result2.firstArgVals == unordered_set<int>({3}));
      auto result3 = (*test.queryT({StmtType::None, 0}, {StmtType::None, i}));
      REQUIRE(result3.firstArgVals == unordered_set<int>({1, 2, 3}));
    }
  }
  auto result4 = (*test.queryT({StmtType::Assign, 0}, {StmtType::None, 8}));
  REQUIRE(result4.firstArgVals == unordered_set<int>({}));
}

TEST_CASE("ParentQueryHandler parentStar(stmtType,stmtType)") {
  auto test = parentTTest();

  auto result1 = *test.queryT({StmtType::While, 0}, {StmtType::If, 0});
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 3}, {2, 3}}));
  auto result2 = *test.queryT({StmtType::While, 0}, {StmtType::While, 0});
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}}));
  auto result3 = *test.queryT({StmtType::While, 0}, {StmtType::Assign, 0});
  REQUIRE(result3.pairVals ==
          pair_set<int, int>({{1, 4}, {1, 6}, {1, 7}, {2, 4}, {2, 6}}));
  auto result4 = *test.queryT({StmtType::If, 0}, {StmtType::None, 0});
  REQUIRE(result4.pairVals == pair_set<int, int>({{3, 4}, {3, 5}, {3, 6}}));
  auto result5 = *test.queryT({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result5.pairVals == pair_set<int, int>({{1, 5}, {2, 5}, {3, 5}}));
  auto result6 = *test.queryT({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result6.pairVals == pair_set<int, int>({{1, 2},
                                                  {1, 3},
                                                  {1, 4},
                                                  {1, 5},
                                                  {1, 6},
                                                  {1, 7},
                                                  {2, 3},
                                                  {2, 4},
                                                  {2, 5},
                                                  {2, 6},
                                                  {3, 4},
                                                  {3, 5},
                                                  {3, 6}}));

  auto result7 = *test.queryT({StmtType::Assign, 0}, {StmtType::None, 0});
  REQUIRE(result7.pairVals == pair_set<int, int>({}));
  auto result8 = *test.queryT({StmtType::None, 0}, {StmtType::Print, 0});
  REQUIRE(result8.pairVals == pair_set<int, int>({}));
}

TEST_CASE("ParentQueryHandler parentStar(_,stmtType)") {
  auto test = parentTTest();
  // 1(->2(->3(->4,5|6)),7),8
  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::If, 0});
  REQUIRE(result1.secondArgVals == unordered_set<int>({3}));

  auto result2 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result2.secondArgVals == unordered_set<int>({2, 3, 4, 5, 6, 7}));
}

TEST_CASE("ParentQueryHandler parentStar(_,stmtNum)") {
  auto test = parentTTest();
  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 5});
  REQUIRE(result1.isEmpty == false);

  auto result2 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 1});
  REQUIRE(result2.isEmpty == true);
}
