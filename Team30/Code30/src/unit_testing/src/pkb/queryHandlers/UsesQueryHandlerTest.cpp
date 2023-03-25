#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/UsesQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "StructureMappingProviderStub.h"
#include "EntityMappingProviderStub.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

static unique_ptr<StructureMappingProviderStub>
setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(1, StmtType::Assign);
  provider->stmtNumToType.set(2, StmtType::Assign);
  provider->stmtNumToType.set(3, StmtType::Assign);
  provider->stmtNumToType.set(4, StmtType::Print);
  provider->stmtNumToType.set(5, StmtType::Read);
  provider->stmtNumToType.set(6, StmtType::If);
  provider->stmtNumToType.set(7, StmtType::While);
  provider->stmtNumToType.set(8, StmtType::Call);

  provider->stmtTypeToNum.set(StmtType::Assign, 1);
  provider->stmtTypeToNum.set(StmtType::Assign, 2);
  provider->stmtTypeToNum.set(StmtType::Assign, 3);
  provider->stmtTypeToNum.set(StmtType::Print, 4);
  provider->stmtTypeToNum.set(StmtType::Read, 5);
  provider->stmtTypeToNum.set(StmtType::If, 6);
  provider->stmtTypeToNum.set(StmtType::While, 7);
  provider->stmtTypeToNum.set(StmtType::Call, 8);

  provider->allStmts = {1, 2, 3, 4, 5, 6, 7, 8};
  return provider;
}

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->variableTable.set(1, "x");
  provider->variableTable.set(2, "y");
  provider->variableTable.set(3, "z");
  provider->variableTable.set(4, "w");
  provider->procedureTable.set(1, "main");
  provider->procedureTable.set(2, "foo");
  provider->procedureTable.set(3, "goo");
  provider->allVariables = {"x", "y", "z", "w"};
  provider->allProcedures = {"main", "foo", "goo"};
  return provider;
}

struct usesTest {
  shared_ptr<UsesTable> table = make_shared<UsesTable>();
  shared_ptr<UsesRevTable> reverseTable = make_shared<UsesRevTable>();
  shared_ptr<UsesPTable> pTable = make_shared<UsesPTable>();
  shared_ptr<UsesPRevTable>
      reversePTable = make_shared<UsesPRevTable>();
  unique_ptr<UsesStorage>
      store = make_unique<UsesStorage>(table.get(), reverseTable.get());
  unique_ptr<UsesPStorage>
      pStore = make_unique<UsesPStorage>(pTable.get(), reversePTable.get());
  unique_ptr<StructureMappingProviderStub>
      structureProvider = setUpStructureMappingProvider();
  unique_ptr<EntityMappingProviderStub>
      entityProvider = setUpEntityMappingProvider();
  unique_ptr<StmtPredicateFactory>
      stmtPredFactory =
      make_unique<StmtPredicateFactory>(structureProvider.get());
  unique_ptr<EntityPredicateFactory>
      entPredFactory = make_unique<EntityPredicateFactory>();
  unique_ptr<PkbStmtEntQueryInvoker> stmtEntInvoker =
      make_unique<PkbStmtEntQueryInvoker>(
          structureProvider.get(),
          stmtPredFactory.get(),
          entPredFactory.get());
  unique_ptr<PkbEntEntQueryInvoker> entEntInvoker =
      make_unique<PkbEntEntQueryInvoker>(
          entityProvider.get(),
          entPredFactory.get());
  UsesQueryHandler
      handler = UsesQueryHandler(
      stmtEntInvoker.get(),
      entEntInvoker.get(),
      store.get(),
      pStore.get());
  unique_ptr<QueryResult<StmtValue, EntityValue>> query(
      StmtRef leftArg, EntityRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }

  unique_ptr<QueryResult<EntityValue, EntityValue>> query(
      EntityRef leftArg, EntityRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }
};

/** Uses(StmtRef, EntityRef) */

// Both args known
TEST_CASE("UsesQueryHandler Uses(stmtNum, variableName)") {
  auto test = usesTest();
  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(3, "z");

  auto result = test.query({StmtType::None, 1}, {EntityType::None, "x"});
  REQUIRE(result.get()->isEmpty == false);
  REQUIRE(result.get()->firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result.get()->pairVals == pair_set<int, string>({{1, "x"}}));
}

// Only arg1 known
TEST_CASE("UsesQueryHandler Uses(stmtNum, variableType)") {
  auto test = usesTest();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result = test.query({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result.get()->isEmpty == false);
  REQUIRE(result.get()->firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.get()->secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.get()->pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(stmtNum, _)") {
  auto test = usesTest();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result = test.query({StmtType::None, 1}, {EntityType::None, ""});
  REQUIRE(result.get()->isEmpty == false);
  REQUIRE(result.get()->firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.get()->secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.get()->pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(stmtNum, constant)") {
  auto test = usesTest();

  test.table->set(1, "x");

  auto result = test.query({StmtType::None, 1}, {EntityType::Constant, ""});
  REQUIRE(result.get()->isEmpty == true);
}

// Only arg2 known

TEST_CASE("UsesQueryHandler Uses(type, variableName), assign, print") {
  auto test = usesTest();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("x", 2);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("z", 3);
  test.reverseTable->set("x", 4);

  auto result1 = test.query(
      {StmtType::Assign, 0},
      {EntityType::Variable, "x"});

  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.get()->pairVals == pair_set<int, string>({{1, "x"}, {2, "x"}}));

  auto result2 = test.query({StmtType::Print, 0}, {EntityType::Variable, "x"});

  REQUIRE(result2.get()->isEmpty == false);
  REQUIRE(result2.get()->firstArgVals == unordered_set<int>({4}));
  REQUIRE(result2.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result2.get()->pairVals == pair_set<int, string>({{4, "x"}}));
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), if, while") {
  auto test = usesTest();

  test.reverseTable->set("x", 6);
  test.reverseTable->set("y", 6);
  test.reverseTable->set("y", 7);

  auto result1 = test.query({StmtType::If, 0}, {EntityType::Variable, "y"});

  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<int>({6}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result1.get()->pairVals == pair_set<int, string>({{6, "y"}}));

  auto result2 = test.query({StmtType::While, 0}, {EntityType::Variable, "y"});

  REQUIRE(result2.get()->isEmpty == false);
  REQUIRE(result2.get()->firstArgVals == unordered_set<int>({7}));
  REQUIRE(result2.get()->secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result2.get()->pairVals == pair_set<int, string>({{7, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), read") {
  auto test = usesTest();

  test.reverseTable->set("x", 5);  // should not happen

  auto result1 = test.query({StmtType::Read, 0}, {EntityType::Variable, "x"});

  REQUIRE(result1.get()->isEmpty == true);
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), stmt") {
  auto test = usesTest();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("x", 6);

  auto result1 = test.query({StmtType::None, 0}, {EntityType::Variable, "x"});

  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<int, string>({{1, "x"}, {4, "x"}, {6, "x"}}));
}

// Both args unknown

TEST_CASE("UsesQueryHandler Uses(stmtType, varType)") {
  auto test = usesTest();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(2, "y");
  test.table->set(3, "x");
  test.table->set(4, "x");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 3);
  test.reverseTable->set("x", 4);

  auto result1 = test.query({StmtType::Assign, 0}, {EntityType::Variable, ""});

  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<int, string>({{1, "x"}, {1, "z"}, {2, "y"}, {3, "x"}}));
}

TEST_CASE("UsesQueryHandler Uses(statement, _)") {
  auto test = usesTest();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(4, "x");
  test.table->set(6, "y");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("z", 1);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("y", 6);

  auto result1 = test.query({StmtType::None, 0}, {EntityType::None, ""});

  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<int, string>({{1, "x"}, {1, "z"}, {4, "x"}, {6, "y"}}));
}

TEST_CASE("UsesQueryHandler call statement") {
  auto test = usesTest();

  test.table->set(8, "x");
  test.table->set(8, "y");
  test.table->set(1, "z");
  test.reverseTable->set("x", 8);
  test.reverseTable->set("y", 8);
  test.reverseTable->set("z", 1);

  // arg1 known
  auto result1 = test.query({StmtType::None, 8}, {EntityType::None, ""});
  REQUIRE(result1.get()->pairVals == pair_set<int, string>({{8, "x"}, {8, "y"}}));
  // arg2 known
  auto result2 = test.query({StmtType::Call, 0}, {EntityType::Variable, ""});

  REQUIRE(result2.get()->pairVals == pair_set<int, string>({{8, "x"}, {8, "y"}}));
  // Both args unknown
  auto result3 = test.query({StmtType::None, 0}, {EntityType::None, ""});
  REQUIRE(result3.get()->pairVals ==
      pair_set<int, string>({{8, "x"}, {8, "y"}, {1, "z"}}));
}

/** Uses(EntityRef, EntityRef) */
// Both args known
TEST_CASE("UsesQueryHandler Uses(procedureName, variableName)") {
  auto test = usesTest();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.query(
      {EntityType::None, "main"},
      {EntityType::None, "x"});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.get()->pairVals == pair_set<string, string>({{"main", "x"}}));

  auto result2 = test.query(
      {EntityType::None, "main"},
      {EntityType::None, "z"});
  REQUIRE(result2.get()->isEmpty == true);

  auto result3 = test.query(
      {EntityType::None, "foo"},
      {EntityType::None, "z"});
  REQUIRE(result3.get()->pairVals == pair_set<string, string>({{"foo", "z"}}));
}

// Only arg1 known
TEST_CASE("UsesQueryHandler Uses(procedureName, type)") {
  auto test = usesTest();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.query(
      {EntityType::None, "main"},
      {EntityType::Variable, ""});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<string, string>({{"main", "x"}, {"main", "y"}}));

  auto result2 = test.query(
      {EntityType::None, "goo"},
      {EntityType::Variable, ""});
  REQUIRE(result2.get()->isEmpty == true);
}

// Only arg2 known
TEST_CASE("UsesQueryHandler Uses(type, variable)") {
  auto test = usesTest();

  test.reversePTable->set("x", "main");
  test.reversePTable->set("x", "foo");
  test.reversePTable->set("y", "main");
  test.reversePTable->set("y", "goo");

  auto result1 = test.query(
      {EntityType::Procedure, ""},
      {EntityType::Variable, "x"});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<string, string>({{"main", "x"}, {"foo", "x"}}));

  // invalid arg1
  auto result2 =
      test.query({EntityType::None, ""}, {EntityType::None, "y"});
  REQUIRE(result2.get()->isEmpty == true);
}

// Both args unknown
TEST_CASE("UsesQueryHandler Uses(type, type)") {
  auto test = usesTest();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.query(
      {EntityType::Procedure, ""},
      {EntityType::None, ""});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.get()->pairVals ==
      pair_set<string, string>(
          {{"main", "x"}, {"main", "y"}, {"foo", "z"}, {"foo", "y"}}));

  // invalid arg1
  auto result2 = test.query(
      {EntityType::None, ""},
      {EntityType::Variable, ""});
  REQUIRE(result2.get()->isEmpty == true);
}

// printDeclaration
TEST_CASE("UsesQueryHandler getPrintDeclarations(printStmt)") {
  auto test = usesTest();
  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(3, "z");
  test.table->set(4, "y");
  test.table->set(5, "test");

  REQUIRE(test.handler.getPrintDeclarations(5) == "test");
}
