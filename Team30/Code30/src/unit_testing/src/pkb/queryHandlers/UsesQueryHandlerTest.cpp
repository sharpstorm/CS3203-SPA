#include <memory>
#include <unordered_set>
#include <utility>

#include "EntityMappingProviderStub.h"
#include "StructureMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/UsesQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"

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

struct usesTestInit {
  shared_ptr<HashKeySetTable<int, string>> table;
  shared_ptr<HashKeySetTable<string, int>> reverseTable;
  unique_ptr<UsesStorage> store;
  shared_ptr<HashKeySetTable<string, string>> pTable;
  shared_ptr<HashKeySetTable<string, string>> reversePTable;
  unique_ptr<UsesPStorage> pStore;
  unique_ptr<StructureMappingProviderStub> structureProvider;
  unique_ptr<EntityMappingProviderStub> entityProvider;
  unique_ptr<PredicateFactory> factory;
  UsesQueryHandler handler;

  usesTestInit()
      : table(make_shared<HashKeySetTable<int, string>>()),
        reverseTable(make_shared<HashKeySetTable<string, int>>()),
        store(make_unique<UsesStorage>(table, reverseTable)),
        pTable(make_shared<HashKeySetTable<string, string>>()),
        reversePTable(make_shared<HashKeySetTable<string, string>>()),
        pStore(make_unique<UsesPStorage>(pTable, reversePTable)),
        structureProvider(setUpStructureMappingProvider()),
        entityProvider(setUpEntityMappingProvider()),
        factory(make_unique<PredicateFactory>(structureProvider.get(),
                                              entityProvider.get())),
        handler(UsesQueryHandler(store.get(), pStore.get(), factory.get(),
                                 structureProvider.get(),
                                 entityProvider.get())){};
};

/** Uses(StmtRef, EntityRef) */

// Both args known
TEST_CASE("UsesQueryHandler Uses(stmtNum, variableName)") {
  auto test = usesTestInit();
  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(3, "z");

  auto result =
      test.handler.queryUses({StmtType::None, 1}, {EntityType::None, "x"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}}));
}

// Only arg1 known

TEST_CASE("UsesQueryHandler Uses(stmtNum, variableType)") {
  auto test = usesTestInit();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result =
      test.handler.queryUses({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(stmtNum, _)") {
  auto test = usesTestInit();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result =
      test.handler.queryUses({StmtType::None, 1}, {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(stmtNum, constant)") {
  auto test = usesTestInit();

  test.table->set(1, "x");

  auto result =
      test.handler.queryUses({StmtType::None, 1}, {EntityType::Constant, ""});
  REQUIRE(result.isEmpty == true);
}

// Only arg2 known

TEST_CASE("UsesQueryHandler Uses(type, variableName), assign, print") {
  auto test = usesTestInit();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("x", 2);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("z", 3);
  test.reverseTable->set("x", 4);

  auto result1 = test.handler.queryUses({StmtType::Assign, 0},
                                        {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals == pair_set<int, string>({{1, "x"}, {2, "x"}}));

  auto result2 =
      test.handler.queryUses({StmtType::Print, 0}, {EntityType::Variable, "x"});

  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<int>({4}));
  REQUIRE(result2.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result2.pairVals == pair_set<int, string>({{4, "x"}}));
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), if, while") {
  auto test = usesTestInit();

  test.reverseTable->set("x", 6);
  test.reverseTable->set("y", 6);
  test.reverseTable->set("y", 7);

  auto result1 =
      test.handler.queryUses({StmtType::If, 0}, {EntityType::Variable, "y"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result1.pairVals == pair_set<int, string>({{6, "y"}}));

  auto result2 =
      test.handler.queryUses({StmtType::While, 0}, {EntityType::Variable, "y"});

  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<int>({7}));
  REQUIRE(result2.secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result2.pairVals == pair_set<int, string>({{7, "y"}}));
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), read") {
  auto test = usesTestInit();

  test.reverseTable->set("x", 5);  // should not happen

  auto result1 =
      test.handler.queryUses({StmtType::Read, 0}, {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == true);
}

TEST_CASE("UsesQueryHandler Uses(type, variableName), stmt") {
  auto test = usesTestInit();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("x", 6);

  auto result1 =
      test.handler.queryUses({StmtType::None, 0}, {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals ==
          pair_set<int, string>({{1, "x"}, {4, "x"}, {6, "x"}}));
}

// Both args unknown

TEST_CASE("UsesQueryHandler Uses(stmtType, varType)") {
  auto test = usesTestInit();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(2, "y");
  test.table->set(3, "x");
  test.table->set(4, "x");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 3);
  test.reverseTable->set("x", 4);

  auto result1 =
      test.handler.queryUses({StmtType::Assign, 0}, {EntityType::Variable, ""});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals ==
          pair_set<int, string>({{1, "x"}, {1, "z"}, {2, "y"}, {3, "x"}}));
}

TEST_CASE("UsesQueryHandler Uses(statement, _)") {
  auto test = usesTestInit();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(4, "x");
  test.table->set(6, "y");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("z", 1);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("y", 6);

  auto result1 =
      test.handler.queryUses({StmtType::None, 0}, {EntityType::None, ""});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals ==
          pair_set<int, string>({{1, "x"}, {1, "z"}, {4, "x"}, {6, "y"}}));
}

TEST_CASE("UsesQueryHandler call statement") {
  auto test = usesTestInit();

  test.table->set(8, "x");
  test.table->set(8, "y");
  test.table->set(1, "z");
  test.reverseTable->set("x", 8);
  test.reverseTable->set("y", 8);
  test.reverseTable->set("z", 1);

  // arg1 known
  auto result1 =
      test.handler.queryUses({StmtType::None, 8}, {EntityType::None, ""});
  REQUIRE(result1.pairVals == pair_set<int, string>({{8, "x"}, {8, "y"}}));
  // arg2 known
  auto result2 =
      test.handler.queryUses({StmtType::Call, 0}, {EntityType::Variable, ""});

  REQUIRE(result2.pairVals == pair_set<int, string>({{8, "x"}, {8, "y"}}));
  // Both args unknown
  auto result3 =
      test.handler.queryUses({StmtType::None, 0}, {EntityType::None, ""});
  REQUIRE(result3.pairVals ==
          pair_set<int, string>({{8, "x"}, {8, "y"}, {1, "z"}}));
}

/** Uses(EntityRef, EntityRef) */
// Both args known
TEST_CASE("UsesQueryHandler Uses(procedureName, variableName)") {
  auto test = usesTestInit();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.handler.queryUses({EntityType::None, "main"},
                                        {EntityType::None, "x"});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals == pair_set<string, string>({{"main", "x"}}));

  auto result2 = test.handler.queryUses({EntityType::None, "main"},
                                        {EntityType::None, "z"});
  REQUIRE(result2.isEmpty == true);

  auto result3 = test.handler.queryUses({EntityType::None, "foo"},
                                        {EntityType::None, "z"});
  REQUIRE(result3.pairVals == pair_set<string, string>({{"foo", "z"}}));
}

// Only arg1 known
TEST_CASE("UsesQueryHandler Uses(procedureName, type)") {
  auto test = usesTestInit();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.handler.queryUses({EntityType::None, "main"},
                                        {EntityType::Variable, ""});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result1.pairVals ==
          pair_set<string, string>({{"main", "x"}, {"main", "y"}}));

  auto result2 = test.handler.queryUses({EntityType::None, "goo"},
                                        {EntityType::Variable, ""});
  REQUIRE(result2.isEmpty == true);
}

// Only arg2 known
TEST_CASE("UsesQueryHandler Uses(type, variable)") {
  auto test = usesTestInit();

  test.reversePTable->set("x", "main");
  test.reversePTable->set("x", "foo");
  test.reversePTable->set("y", "main");
  test.reversePTable->set("y", "goo");

  auto result1 = test.handler.queryUses({EntityType::Procedure, ""},
                                        {EntityType::Variable, "x"});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals ==
          pair_set<string, string>({{"main", "x"}, {"foo", "x"}}));

  // invalid arg1
  auto result2 =
      test.handler.queryUses({EntityType::None, ""}, {EntityType::None, "y"});
  REQUIRE(result2.isEmpty == true);
}

// Both args unknown
TEST_CASE("UsesQueryHandler Uses(type, type)") {
  auto test = usesTestInit();

  test.pTable->set("main", "x");
  test.pTable->set("main", "y");
  test.pTable->set("foo", "y");
  test.pTable->set("foo", "z");

  auto result1 = test.handler.queryUses({EntityType::Procedure, ""},
                                        {EntityType::None, ""});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals ==
          pair_set<string, string>(
              {{"main", "x"}, {"main", "y"}, {"foo", "z"}, {"foo", "y"}}));

  // invalid arg1
  auto result2 = test.handler.queryUses({EntityType::None, ""},
                                        {EntityType::Variable, ""});
  REQUIRE(result2.isEmpty == true);
}
