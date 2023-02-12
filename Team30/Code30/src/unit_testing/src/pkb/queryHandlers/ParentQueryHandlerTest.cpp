#include <memory>
#include <unordered_set>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/tables/ContiguousSetTable.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::unordered_set;

class ParentQHStructureProviderStub : public StructureMappingProvider {
 public:
  ParentQHStructureProviderStub()
      : StructureMappingProvider(nullptr, nullptr) {};

  bool isStatementOfType(int s, StmtType stmtType) const override {
    switch (s) {
      case 2:
      case 3:return stmtType == StmtType::Assign;
      case 4:
      case 5:return stmtType == StmtType::Read;
      case 11:
      case 14:return stmtType == StmtType::If;
      case 12:
      case 16:return stmtType == StmtType::While;
      default:return false;
    }
  }
  unordered_set<int> getStatementsOfType(StmtType stmtType) const override {
    switch (stmtType) {
      case StmtType::Assign:return {2, 3};
      case StmtType::Read:return {4, 5};
      case StmtType::If:return {11, 14};
      case StmtType::While:return {12, 16};
      default:return unordered_set<int>();
    }
  }
};

/* Parent */

TEST_CASE("ParentQueryHandler parent(stmtNum,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  ParentQueryHandler
      handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 4);

  REQUIRE(
      handler.queryParent({StmtType::None, 1}, {StmtType::None, 2}).isEmpty ==
          false);
  REQUIRE(
      handler.queryParent({StmtType::None, 2}, {StmtType::None, 1}).isEmpty ==
          true);
  REQUIRE(
      handler.queryParent({StmtType::None, 4}, {StmtType::None, 4}).isEmpty ==
          true);
  REQUIRE(
      handler.queryParent({StmtType::None, 1}, {StmtType::None, 1}).isEmpty ==
          true);
}

TEST_CASE("ParentQueryHandler parent(stmtNum,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 3);
  table->set(3, 5);

  auto result1 =
      handler.queryParent({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 = handler.queryParent({StmtType::None, 2}, {StmtType::Read, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parent(stmtType, stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  reverseTable->set(5, 2);
  reverseTable->set(6, 5);
  reverseTable->set(8, 6);

  auto result1 =
      handler.queryParent({StmtType::Assign, 0}, {StmtType::None, 5});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}}));

  auto result2 = handler.queryParent({StmtType::Read, 0}, {StmtType::None, 8});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parent(stmtType, stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(2, 5);
  table->set(3, 4);
  table->set(5, 6);

  auto result1 =
      handler.queryParent({StmtType::Assign, 0}, {StmtType::Read, 0});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({5, 4}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{2, 5}, {3, 4}}));
}

/* ParentStar */
TEST_CASE("ParentQueryHandler parentStar(stmtNum,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(1, 2);
  table->set(2, 5);

  REQUIRE(handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 2})
              .isEmpty == false);
  REQUIRE(handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 5})
              .isEmpty == false);
  REQUIRE(handler.queryParentStar({StmtType::None, 1}, {StmtType::None, 1})
              .isEmpty == true);
  REQUIRE(handler.queryParentStar({StmtType::None, 5}, {StmtType::None, 2})
              .isEmpty == true);
}

TEST_CASE("ParentQueryHandler parentStar(stmtNum,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(10, 11);
  table->set(11, 12);
  table->set(12, 16);

  auto result1 =
      handler.queryParentStar({StmtType::None, 10}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({10}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{10, 12}, {10, 16}}));

  auto result2 =
      handler.queryParentStar({StmtType::None, 12}, {StmtType::If, 0});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parentStar(stmtType,stmtNum)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  reverseTable->set(12, 11);
  reverseTable->set(13, 12);
  reverseTable->set(14, 13);

  auto result1 =
      handler.queryParentStar({StmtType::If, 0}, {StmtType::None, 13});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({13}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{11, 13}}));

  auto result2 =
      handler.queryParentStar({StmtType::If, 0}, {StmtType::None, 11});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentQueryHandler parentStar(stmtType,stmtType)") {
  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = make_unique<ParentStorage>(table, reverseTable);
  auto provider = make_unique<ParentQHStructureProviderStub>();
  auto factory = make_unique<PredicateFactory>(provider.get(), nullptr);
  auto handler = ParentQueryHandler(store.get(), factory.get(), provider.get());

  table->set(11, 12);
  table->set(12, 14);
  table->set(14, 15);
  table->set(15, 16);

  auto result1 =
      handler.queryParentStar({StmtType::If, 0}, {StmtType::While, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({11, 14}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({12, 16}));
  REQUIRE(result1.pairVals ==
      pair_set<int, int>({{11, 12}, {11, 16}, {14, 16}}));
}
