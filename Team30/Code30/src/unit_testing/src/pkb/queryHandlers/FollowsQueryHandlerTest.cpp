#include <memory>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "StructureMappingProviderStub.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::unordered_set;
using std::unique_ptr;

static std::unique_ptr<StructureMappingProviderStub> setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(2, StmtType::Assign);
  provider->stmtNumToType.set(3, StmtType::Assign);
  provider->stmtNumToType.set(4, StmtType::Read);
  provider->stmtNumToType.set(5, StmtType::Read);
  provider->stmtNumToType.set(11, StmtType::If);
  provider->stmtNumToType.set(14, StmtType::If);
  provider->stmtNumToType.set(12, StmtType::While);
  provider->stmtNumToType.set(16, StmtType::While);

  provider->stmtTypeToNum.set(StmtType::Assign, 2);
  provider->stmtTypeToNum.set(StmtType::Assign, 3);
  provider->stmtTypeToNum.set(StmtType::Read, 4);
  provider->stmtTypeToNum.set(StmtType::Read, 5);
  provider->stmtTypeToNum.set(StmtType::If, 11);
  provider->stmtTypeToNum.set(StmtType::If, 14);
  provider->stmtTypeToNum.set(StmtType::While, 12);
  provider->stmtTypeToNum.set(StmtType::While, 16);

  provider->allStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  return provider;
}

struct followsTest {
  shared_ptr<FollowsTable> table = make_shared<FollowsTable>();
  shared_ptr<FollowsRevTable> reverseTable = make_shared<FollowsRevTable>();
  unique_ptr<FollowsStorage>
      store = make_unique<FollowsStorage>(table.get(), reverseTable.get());
  unique_ptr<FollowsTStorage>
      storeT = make_unique<FollowsTStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub>
      structureProvider = setUpStructureMappingProvider();
  unique_ptr<StmtPredicateFactory>
      factory = make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<PkbStmtStmtQueryInvoker> invoker =
      make_unique<PkbStmtStmtQueryInvoker>(
          structureProvider.get(),
          factory.get());
  FollowsQueryHandler handler = FollowsQueryHandler(invoker.get(), store.get());
  FollowsQueryHandler
      handlerT = FollowsQueryHandler(invoker.get(), storeT.get());

  QueryResult<StmtValue, StmtValue> query(
      StmtRef leftArg, StmtRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }

  QueryResult<StmtValue, StmtValue> queryT(
      StmtRef leftArg, StmtRef rightArg) {
    return handlerT.query(&leftArg, &rightArg);
  }
};

/* Follows */

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtNum)") {
  auto test = followsTest();

  test.table->set(1, 2);
  test.table->set(2, 4);

  REQUIRE(test.query(
      {StmtType::None, 1},
      {StmtType::None, 2}).isEmpty == false);
  REQUIRE(test.query(
      {StmtType::None, 2},
      {StmtType::None, 1}).isEmpty == true);
  REQUIRE(test.query(
      {StmtType::None, 4},
      {StmtType::None, 4}).isEmpty == true);
  REQUIRE(test.query(
      {StmtType::None, 1},
      {StmtType::None, 1}).isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtType)") {
  auto test = followsTest();

  test.table->set(1, 2);
  test.table->set(2, 3);
  test.table->set(3, 5);

  auto result1 = test.query({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = test.query({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtNum)") {
  auto test = followsTest();

  test.reverseTable->set(5, 2);
  test.reverseTable->set(6, 5);
  test.reverseTable->set(8, 6);

  auto result1 = test.query({StmtType::Assign, 0}, {StmtType::None, 5});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}}));

  auto result2 =
      test.query({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtType)") {
  auto test = followsTest();

  test.table->set(2, 5);
  test.table->set(3, 4);
  test.table->set(5, 6);

  auto result1 = test.query({StmtType::Assign, 0}, {StmtType::Read, 0});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5, 4}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}, {3, 4}}));
}

/* FollowsStar */
TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtNum)") {
  auto test = followsTest();

  test.table->set(1, 2);
  test.table->set(2, 5);

  REQUIRE(test.queryT({StmtType::None, 1}, {StmtType::None, 2})
      .isEmpty == false);
  REQUIRE(test.queryT({StmtType::None, 1}, {StmtType::None, 5})
      .isEmpty == false);
  REQUIRE(test.queryT({StmtType::None, 1}, {StmtType::None, 1})
      .isEmpty == true);
  REQUIRE(test.queryT({StmtType::None, 5}, {StmtType::None, 2})
      .isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtType)") {
  auto test = followsTest();

  test.table->set(10, 11);
  test.table->set(11, 12);
  test.table->set(12, 16);

  auto result1 = test.queryT(
      {StmtType::None, 10},
      {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({10}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{10, 12}, {10, 16}}));

  auto result2 = test.queryT({StmtType::None, 12}, {StmtType::If, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtNum)") {
  auto test = followsTest();

  test.reverseTable->set(12, 11);
  test.reverseTable->set(13, 12);
  test.reverseTable->set(14, 13);

  auto result1 = test.queryT({StmtType::If, 0}, {StmtType::None, 13});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({13}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 13}}));

  auto result2 = test.queryT({StmtType::If, 0}, {StmtType::None, 11});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtType)") {
  auto test = followsTest();

  test.table->set(11, 12);
  test.table->set(12, 14);
  test.table->set(14, 15);
  test.table->set(15, 16);

  auto result1 = test.queryT({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11, 14}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals ==
      pair_set<int, int>({{11, 12}, {11, 16}, {14, 16}}));

  auto result2 = test.queryT({StmtType::None, 0}, {StmtType::While, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<int>({11, 12, 14, 15}));
  REQUIRE(result2.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result2.pairVals ==
      pair_set<int, int>({{11, 12}, {11, 16}, {12, 16}, {14, 16}, {15, 16}}));

  auto result3 = test.queryT({StmtType::If, 0}, {StmtType::None, 0});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.firstArgVals == unordered_set<int>({11, 14}));
  REQUIRE(result3.secondArgVals == unordered_set<int>({12, 12, 14, 15, 16}));
  REQUIRE(result3.pairVals ==
      pair_set<int, int>(
          {{11, 12}, {11, 14}, {11, 15}, {11, 16}, {14, 15},
           {14, 16}}));
}

