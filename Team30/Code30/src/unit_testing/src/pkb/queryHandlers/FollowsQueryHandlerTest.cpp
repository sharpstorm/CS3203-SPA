#include <memory>
#include <utility>

#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::unique_ptr;

static std::unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.insert(1, StmtType::Assign);
  provider->stmtNumToType.insert(2, StmtType::Assign);
  provider->stmtNumToType.insert(3, StmtType::Assign);
  provider->stmtNumToType.insert(4, StmtType::Read);
  provider->stmtNumToType.insert(5, StmtType::Read);
  provider->stmtNumToType.insert(6, StmtType::Print);
  provider->stmtNumToType.insert(11, StmtType::If);
  provider->stmtNumToType.insert(14, StmtType::If);
  provider->stmtNumToType.insert(12, StmtType::While);
  provider->stmtNumToType.insert(16, StmtType::While);

  provider->stmtTypeToNum.insert(StmtType::Assign, 1);
  provider->stmtTypeToNum.insert(StmtType::Assign, 2);
  provider->stmtTypeToNum.insert(StmtType::Assign, 3);
  provider->stmtTypeToNum.insert(StmtType::Read, 4);
  provider->stmtTypeToNum.insert(StmtType::Read, 5);
  provider->stmtTypeToNum.insert(StmtType::Print, 6);
  provider->stmtTypeToNum.insert(StmtType::If, 11);
  provider->stmtTypeToNum.insert(StmtType::If, 14);
  provider->stmtTypeToNum.insert(StmtType::While, 12);
  provider->stmtTypeToNum.insert(StmtType::While, 16);

  provider->allStmts = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  return provider;
}

struct followsQHTest {
  shared_ptr<FollowsTable> table = make_shared<FollowsTable>();
  shared_ptr<FollowsRevTable> reverseTable = make_shared<FollowsRevTable>();
  unique_ptr<FollowsStorage> store =
      make_unique<FollowsStorage>(table.get(), reverseTable.get());
  unique_ptr<FollowsTStorage> storeT =
      make_unique<FollowsTStorage>(table.get(), reverseTable.get());
  unique_ptr<StructureMappingProviderStub> structureProvider =
      setUpStructureMappingProvider();
  unique_ptr<StmtPredicateFactory> factory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<PkbStmtStmtQueryInvoker> invoker =
      make_unique<PkbStmtStmtQueryInvoker>(structureProvider.get(),
                                           factory.get());
  FollowsQueryHandler handler = FollowsQueryHandler(invoker.get(), store.get());
  FollowsQueryHandler handlerT =
      FollowsQueryHandler(invoker.get(), storeT.get());

  QueryResultPtr<StmtValue, StmtValue> query(StmtRef leftArg,
                                             StmtRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }

  QueryResultPtr<StmtValue, StmtValue> queryT(StmtRef leftArg,
                                              StmtRef rightArg) {
    return handlerT.query(&leftArg, &rightArg);
  }
};

/* Follows */

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtNum)") {
  auto test = followsQHTest();

  // 1->2->4->5
  test.table->insert(1, 2);
  test.table->insert(1, 4);
  test.table->insert(1, 5);
  test.table->insert(2, 4);
  test.table->insert(2, 5);
  test.table->insert(4, 5);

  auto res1 = *test.query({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(res1.empty() == false);
  auto res2 = *test.query({StmtType::None, 2}, {StmtType::None, 1});
  REQUIRE(res2.empty() == true);
  auto res3 = *test.query({StmtType::None, 5}, {StmtType::None, 4});
  REQUIRE(res3.empty() == true);
  auto res4 = *test.query({StmtType::None, 1}, {StmtType::None, 1});
  REQUIRE(res4.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtType)") {
  auto test = followsQHTest();

  // 1->2->3->5
  test.table->insert(1, 2);
  test.table->insert(1, 3);
  test.table->insert(1, 5);
  test.table->insert(2, 3);
  test.table->insert(2, 5);
  test.table->insert(3, 5);

  auto result1 = *test.query({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getRightVals() == StmtValueSet({2}));

  auto result2 = *test.query({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtNum)") {
  auto test = followsQHTest();

  // 2->5->6->8
  test.reverseTable->insert(5, 2);
  test.reverseTable->insert(6, 2);
  test.reverseTable->insert(8, 2);
  test.reverseTable->insert(6, 5);
  test.reverseTable->insert(8, 5);
  test.reverseTable->insert(8, 6);

  auto result1 = *test.query({StmtType::Assign, 0}, {StmtType::None, 5});
  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getLeftVals() == StmtValueSet({2}));

  auto result2 = *test.query({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtType)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.table->insert(2, 5);
  test.table->insert(2, 6);
  test.table->insert(5, 6);
  test.table->insert(3, 4);

  auto result1 = *test.query({StmtType::Assign, 0}, {StmtType::Read, 0});

  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getPairVals() ==
          pair_set<StmtValue, StmtValue>({{2, 5}, {3, 4}}));

  auto result2 = *test.query({StmtType::Assign, 0}, {StmtType::Print, 0});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(syn, syn)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result1.getPairVals() ==
          pair_set<StmtValue, StmtValue>({{2, 5}, {5, 6}, {3, 4}}));

  auto result2 = *test.query({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result2.getPairVals() ==
          pair_set<StmtValue, StmtValue>({{2, 5}, {3, 4}}));

  auto result3 = *test.query({StmtType::Assign, 0}, {StmtType::None, 0});
  REQUIRE(result3.getPairVals() ==
          pair_set<StmtValue, StmtValue>({{2, 5}, {3, 4}}));
}

TEST_CASE("FollowsQueryHandler follows(stmtNum, _)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::None, 2}, {StmtType::Wildcard, 0});
  REQUIRE(result1.empty() == false);

  auto result2 = *test.query({StmtType::None, 4}, {StmtType::Wildcard, 0});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(_, stmtNum)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::None, 6});
  REQUIRE(result1.empty() == false);

  auto result2 = *test.query({StmtType::Wildcard, 0}, {StmtType::None, 3});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler follows(syn, _)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::None, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.getLeftVals() == StmtValueSet({2, 5, 3}));

  auto result2 = *test.query({StmtType::Assign, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result2.getLeftVals() == StmtValueSet({2, 3}));
}

TEST_CASE("FollowsQueryHandler follows(_, syn)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result1.getRightVals() == StmtValueSet({5, 6, 4}));

  auto result2 = *test.query({StmtType::Wildcard, 0}, {StmtType::Read, 0});
  REQUIRE(result2.getRightVals() == StmtValueSet({5, 4}));
}

TEST_CASE("FollowsQueryHandler follows(_, _)") {
  auto test = followsQHTest();

  // 2->5->6, 3->4
  test.store->insert(2, 5);
  test.store->insert(2, 6);
  test.store->insert(5, 6);
  test.store->insert(3, 4);

  auto result1 = *test.query({StmtType::Wildcard, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.empty() == false);
}

/* FollowsStar */
TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtNum)") {
  auto test = followsQHTest();

  // 1->2->5
  test.table->insert(1, 2);
  test.table->insert(1, 5);
  test.table->insert(2, 5);

  auto res1 = *test.queryT({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(res1.empty() == false);

  auto res2 = *test.queryT({StmtType::None, 1}, {StmtType::None, 5});
  REQUIRE(res2.empty() == false);

  auto res3 = *test.queryT({StmtType::None, 1}, {StmtType::None, 1});
  REQUIRE(res3.empty() == true);

  auto res4 = *test.queryT({StmtType::None, 5}, {StmtType::None, 2});
  REQUIRE(res4.empty() == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtType)") {
  auto test = followsQHTest();

  // 10->11->12->16
  test.table->insert(10, 11);
  test.table->insert(10, 12);
  test.table->insert(10, 16);
  test.table->insert(11, 12);
  test.table->insert(11, 16);
  test.table->insert(12, 16);

  auto result1 = *test.queryT({StmtType::None, 10}, {StmtType::While, 0});
  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getRightVals() == StmtValueSet({12, 16}));

  auto result2 = *test.queryT({StmtType::None, 12}, {StmtType::If, 0});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtNum)") {
  auto test = followsQHTest();

  // 11->12->13->14
  test.reverseTable->insert(12, 11);
  test.reverseTable->insert(13, 11);
  test.reverseTable->insert(14, 11);
  test.reverseTable->insert(13, 12);
  test.reverseTable->insert(14, 12);
  test.reverseTable->insert(14, 13);

  auto result1 = *test.queryT({StmtType::If, 0}, {StmtType::None, 13});
  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getLeftVals() == StmtValueSet({11}));

  auto result2 = *test.queryT({StmtType::If, 0}, {StmtType::None, 11});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtType)") {
  auto test = followsQHTest();

  // 11->12->14->15->16
  test.table->insert(11, 12);
  test.table->insert(11, 14);
  test.table->insert(11, 15);
  test.table->insert(11, 16);
  test.table->insert(12, 14);
  test.table->insert(12, 15);
  test.table->insert(12, 16);
  test.table->insert(14, 15);
  test.table->insert(14, 16);
  test.table->insert(15, 16);

  auto result1 = *test.queryT({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.empty() == false);
  REQUIRE(result1.getPairVals() ==
          pair_set<StmtValue, StmtValue>({{11, 12}, {11, 16}, {14, 16}}));

  auto result2 = *test.queryT({StmtType::None, 0}, {StmtType::While, 0});
  REQUIRE(result2.empty() == false);
  REQUIRE(result2.getPairVals() ==
          pair_set<StmtValue, StmtValue>(
              {{11, 12}, {11, 16}, {12, 16}, {14, 16}, {15, 16}}));

  auto result3 = *test.queryT({StmtType::If, 0}, {StmtType::None, 0});
  REQUIRE(result3.empty() == false);
  REQUIRE(result3.getPairVals() ==
          pair_set<StmtValue, StmtValue>(
              {{11, 12}, {11, 14}, {11, 15}, {11, 16}, {14, 15}, {14, 16}}));
}

TEST_CASE("FollowsQueryHandler followsStar(syn,syn)") {
  auto test = followsQHTest();

  // 11->12->14->15->16
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(11, 15);
  test.store->insert(11, 16);
  test.store->insert(12, 14);
  test.store->insert(12, 15);
  test.store->insert(12, 16);
  test.store->insert(14, 15);
  test.store->insert(14, 16);
  test.store->insert(15, 16);

  auto result1 = *test.queryT({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result1.getPairVals() == pair_set<StmtValue, StmtValue>({{11, 12},
                                                                   {11, 14},
                                                                   {11, 15},
                                                                   {11, 16},
                                                                   {12, 14},
                                                                   {12, 15},
                                                                   {12, 16},
                                                                   {14, 15},
                                                                   {14, 16},
                                                                   {15, 16}}));
  auto result2 = *test.queryT({StmtType::None, 0}, {StmtType::While, 0});
  REQUIRE(result2.getPairVals() ==
          pair_set<StmtValue, StmtValue>(
              {{11, 12}, {11, 16}, {12, 16}, {14, 16}, {15, 16}}));
  auto result3 = *test.queryT({StmtType::If, 0}, {StmtType::None, 0});
  REQUIRE(result3.getPairVals() ==
          pair_set<StmtValue, StmtValue>(
              {{11, 12}, {11, 14}, {11, 15}, {11, 16}, {14, 15}, {14, 16}}));
}

TEST_CASE("FollowsQueryHandler followsStar(_,stmtNum)") {
  auto test = followsQHTest();

  // 11->12->14
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(12, 14);

  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 14});
  REQUIRE(result1.empty() == false);

  auto result2 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 11});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtNum,_)") {
  auto test = followsQHTest();

  // 11->12->14
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(12, 14);

  auto result1 = *test.queryT({StmtType::None, 11}, {StmtType::Wildcard, 0});
  REQUIRE(result1.empty() == false);

  auto result2 = *test.queryT({StmtType::None, 14}, {StmtType::Wildcard, 0});
  REQUIRE(result2.empty() == true);
}

TEST_CASE("FollowsQueryHandler followsStar(_,syn)") {
  auto test = followsQHTest();

  // 11->12->14, 15->16
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(12, 14);
  test.store->insert(15, 16);

  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::None, 0});
  REQUIRE(result1.getRightVals() == StmtValueSet({12, 14, 16}));

  auto result2 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::While, 0});
  REQUIRE(result2.getRightVals() == StmtValueSet({12, 16}));
}

TEST_CASE("FollowsQueryHandler followsStar(syn,_)") {
  auto test = followsQHTest();

  // 11->12->14, 15->16
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(12, 14);
  test.store->insert(15, 16);

  auto result1 = *test.queryT({StmtType::None, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.getLeftVals() == StmtValueSet({11, 12, 15}));

  auto result2 = *test.queryT({StmtType::If, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result2.getLeftVals() == StmtValueSet({11}));
}

TEST_CASE("FollowsQueryHandler followsStar(_,_)") {
  auto test = followsQHTest();

  // 11->12->14, 15->16
  test.store->insert(11, 12);
  test.store->insert(11, 14);
  test.store->insert(12, 14);
  test.store->insert(15, 16);

  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.empty() == false);
}

TEST_CASE("FollowsQueryHandler followsStar(_,_) is empty") {
  auto test = followsQHTest();

  auto result1 = *test.queryT({StmtType::Wildcard, 0}, {StmtType::Wildcard, 0});
  REQUIRE(result1.empty() == true);
}
