#include <memory>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "StructureMappingProviderStub.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::unordered_set;

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
  return provider;
}

/* Follows */

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 4);

  REQUIRE(
      handler.queryFollows({StmtType::None, 1}, {StmtType::None, 2}).isEmpty ==
          false);
  REQUIRE(
      handler.queryFollows({StmtType::None, 2}, {StmtType::None, 1}).isEmpty ==
          true);
  REQUIRE(
      handler.queryFollows({StmtType::None, 4}, {StmtType::None, 4}).isEmpty ==
          true);
  REQUIRE(
      handler.queryFollows({StmtType::None, 1}, {StmtType::None, 1}).isEmpty ==
          true);
}

TEST_CASE("FollowsQueryHandler follows(stmtNum,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 3);
  table->set(3, 5);

  auto result1 =
      handler.queryFollows({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = handler.queryFollows({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  reverseTable->set(5, 2);
  reverseTable->set(6, 5);
  reverseTable->set(8, 6);

  auto result1 =
      handler.queryFollows({StmtType::Assign, 0}, {StmtType::None, 5});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}}));

  auto result2 = handler.queryFollows({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler follows(stmtType, stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(2, 5);
  table->set(3, 4);
  table->set(5, 6);

  auto result1 =
      handler.queryFollows({StmtType::Assign, 0}, {StmtType::Read, 0});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5, 4}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}, {3, 4}}));
}

/* FollowsStar */
TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 5);

  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 2})
              .isEmpty == false);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 5})
              .isEmpty == false);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 1})
              .isEmpty == true);
  REQUIRE(handler.queryFollowsStar({StmtType::None, 5}, {StmtType::None, 2})
              .isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtNum,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(10, 11);
  table->set(11, 12);
  table->set(12, 16);

  auto result1 =
      handler.queryFollowsStar({StmtType::None, 10}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({10}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{10, 12}, {10, 16}}));

  auto result2 =
      handler.queryFollowsStar({StmtType::None, 12}, {StmtType::If, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  reverseTable->set(12, 11);
  reverseTable->set(13, 12);
  reverseTable->set(14, 13);

  auto result1 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::None, 13});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({13}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 13}}));

  auto result2 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::None, 11});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("FollowsQueryHandler followsStar(stmtType,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<FollowsStorage>(table, reverseTable);
  auto provider = setUpStructureMappingProvider();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler =
      FollowsQueryHandler(store.get(), factory.get(), provider.get());

  table->set(11, 12);
  table->set(12, 14);
  table->set(14, 15);
  table->set(15, 16);

  auto result1 =
      handler.queryFollowsStar({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11, 14}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals ==
      pair_set<int, int>({{11, 12}, {11, 16}, {14, 16}}));
}

// TEST_CASE("FollowsQueryHandler followsStar(_,stmtType)") {
//  auto table = make_shared<ContiguousSetTable<int>>();
//  auto reverseTable = make_shared<ContiguousSetTable<int>>();
//  auto store = new FollowsStorage(table, reverseTable);
//  FollowsQHStructureProviderStub* structureProvider =
//      new FollowsQHStructureProviderStub();
//  PredicateFactory* factory = new PredicateFactory(structureProvider);
//  FollowsQueryHandler handler = FollowsQueryHandler(store, factory, structureProvider);
//
//  table->set(11, 12);
//  table->set(12, 14);
//  table->set(14, 15);
//  table->set(15, 16);
//
//  auto result1 =
//      handler.queryFollowsStar({StmtType::None, 0}, {StmtType::While, 0});
//  REQUIRE(result1.isEmpty == false);
//  REQUIRE(result1.firstArgVals == unordered_set<int>({11, 12, 14, 15}));
//  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
//  REQUIRE(result1.pairVals ==
//          pair_set<int, int>({{11, 12}, {12, 16}, {14, 16}, {15, 16}}));
//}
