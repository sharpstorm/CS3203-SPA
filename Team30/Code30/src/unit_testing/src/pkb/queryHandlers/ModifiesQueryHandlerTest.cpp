#include <memory>
#include <unordered_set>
#include <utility>
#include <string>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/queryHandlers/ModifiesQueryHandler.h"
#include "StructureMappingProviderStub.h"
#include "EntityMappingProviderStub.h"

using std::make_shared;
using std::make_unique;
using std::unique_ptr;
using std::shared_ptr;
using std::unordered_set;
using std::string;

static unique_ptr<StructureMappingProviderStub> setUpStructureMappingProvider() {
  auto provider = make_unique<StructureMappingProviderStub>();
  provider->stmtNumToType.set(1, StmtType::Assign);
  provider->stmtNumToType.set(2, StmtType::Assign);
  provider->stmtNumToType.set(3, StmtType::Assign);
  provider->stmtNumToType.set(4, StmtType::Read);
  provider->stmtNumToType.set(5, StmtType::Print);
  provider->stmtNumToType.set(6, StmtType::If);
  provider->stmtNumToType.set(7, StmtType::While);

  provider->stmtTypeToNum.set(StmtType::Assign, 1);
  provider->stmtTypeToNum.set(StmtType::Assign, 2);
  provider->stmtTypeToNum.set(StmtType::Assign, 3);
  provider->stmtTypeToNum.set(StmtType::Read, 4);
  provider->stmtTypeToNum.set(StmtType::Print, 5);
  provider->stmtTypeToNum.set(StmtType::If, 6);
  provider->stmtTypeToNum.set(StmtType::While, 7);

  provider->allStmts = {1, 2, 3, 4, 5, 6, 7};
  return provider;
}

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->entityTypeToValue.set(EntityType::Variable, "x");
  provider->entityTypeToValue.set(EntityType::Variable, "y");
  provider->entityTypeToValue.set(EntityType::Variable, "z");
  provider->entityTypeToValue.set(EntityType::Variable, "w");
  provider->valueToEntityType.set("x", EntityType::Variable);
  provider->valueToEntityType.set("y", EntityType::Variable);
  provider->valueToEntityType.set("z", EntityType::Variable);
  provider->valueToEntityType.set("w", EntityType::Variable);
  provider->allSymbols = {"x", "y", "z", "w"};
  return provider;
}

struct modifiesTestInit {
  shared_ptr<HashKeySetTable<int, string>> table;
  shared_ptr<HashKeySetTable<string, int>> reverseTable;
  unique_ptr<ModifiesStorage> store;
  unique_ptr<StructureMappingProviderStub> structureProvider;
  unique_ptr<EntityMappingProviderStub> entityProvider;
  unique_ptr<PredicateFactory> factory;
  ModifiesQueryHandler handler;

  modifiesTestInit() :
      table(make_shared<HashKeySetTable<int, string>>()),
      reverseTable(make_shared<HashKeySetTable<string, int>>()),
      store(make_unique<ModifiesStorage>(table, reverseTable)),
      structureProvider(setUpStructureMappingProvider()),
      entityProvider(setUpEntityMappingProvider()),
      factory(make_unique<PredicateFactory>(structureProvider.get(),
                                            entityProvider.get())),
      handler(ModifiesQueryHandler(store.get(),
                                   factory.get(),
                                   structureProvider.get(),
                                   entityProvider.get())) {};
};

/** Modifies(StmtRef, EntityRef) */

// Both args known
TEST_CASE("ModifiesQueryHandler Modifies(stmtNum, variableName)") {
  auto test = modifiesTestInit();
  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(3, "z");

  auto result = test.handler.queryModifies({StmtType::None, 1},
                                           {EntityType::None, "x"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}}));
}

// Only arg1 known

TEST_CASE("ModifiesQueryHandler Modifies(stmtNum, variableType)") {
  auto test = modifiesTestInit();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result = test.handler.queryModifies({StmtType::None, 1},
                                           {EntityType::Variable, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("ModifiesQueryHandler Modifies(stmtNum, _)") {
  auto test = modifiesTestInit();

  test.table->set(1, "x");
  test.table->set(2, "x");
  test.table->set(1, "y");

  auto result = test.handler.queryModifies({StmtType::None, 1},
                                           {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result.secondArgVals == unordered_set<string>({"x", "y"}));
  REQUIRE(result.pairVals == pair_set<int, string>({{1, "x"}, {1, "y"}}));
}

TEST_CASE("ModifiesQueryHandler Modifies(stmtNum, constant)") {
  auto test = modifiesTestInit();

  test.table->set(1, "x");

  auto result = test.handler.queryModifies({StmtType::None, 1},
                                           {EntityType::Constant, ""});
  REQUIRE(result.isEmpty == true);
}

// Only arg2 known

TEST_CASE("ModifiesQueryHandler Modifies(type, variableName), assign, read") {
  auto test = modifiesTestInit();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("x", 2);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("z", 3);
  test.reverseTable->set("x", 4);

  auto result1 = test.handler.queryModifies({StmtType::Assign, 0},
                                            {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals == pair_set<int, string>({{1, "x"}, {2, "x"}}));

  auto result2 = test.handler.queryModifies({StmtType::Read, 0},
                                            {EntityType::Variable, "x"});

  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<int>({4}));
  REQUIRE(result2.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result2.pairVals == pair_set<int, string>({{4, "x"}}));
}

TEST_CASE("ModifiesQueryHandler Modifies(type, variableName), if, while") {
  auto test = modifiesTestInit();

  test.reverseTable->set("x", 6);
  test.reverseTable->set("y", 6);
  test.reverseTable->set("y", 7);

  auto result1 = test.handler.queryModifies({StmtType::If, 0},
                                            {EntityType::Variable, "y"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result1.pairVals == pair_set<int, string>({{6, "y"}}));

  auto result2 = test.handler.queryModifies({StmtType::While, 0},
                                            {EntityType::Variable, "y"});

  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<int>({7}));
  REQUIRE(result2.secondArgVals == unordered_set<string>({"y"}));
  REQUIRE(result2.pairVals == pair_set<int, string>({{7, "y"}}));
}

TEST_CASE("ModifiesQueryHandler Modifies(type, variableName), print") {
  auto test = modifiesTestInit();

  test.reverseTable->set("x", 5); // should not happen

  auto result1 = test.handler.queryModifies({StmtType::Print, 0},
                                            {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == true);
}

TEST_CASE("ModifiesQueryHandler Modifies(type, variableName), stmt") {
  auto test = modifiesTestInit();

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("x", 6);

  auto result1 = test.handler.queryModifies({StmtType::None, 0},
                                            {EntityType::Variable, "x"});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals
              == pair_set<int, string>({{1, "x"}, {4, "x"}, {6, "x"}}));
}

// Both args unknown

TEST_CASE("ModifiesQueryHandler Modifies(stmtType, varType)") {
  auto test = modifiesTestInit();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(2, "y");
  test.table->set(3, "x");
  test.table->set(4, "x");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("x", 3);
  test.reverseTable->set("x", 4);

  auto result1 = test.handler.queryModifies({StmtType::Assign, 0},
                                            {EntityType::Variable, ""});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals
              == pair_set<int, string>({{1, "x"}, {1, "z"}, {2, "y"},
                                        {3, "x"}}));
}

TEST_CASE("ModifiesQueryHandler Modifies(statement, _)") {
  auto test = modifiesTestInit();

  test.table->set(1, "x");
  test.table->set(1, "z");
  test.table->set(4, "x");
  test.table->set(6, "y");

  test.reverseTable->set("x", 1);
  test.reverseTable->set("z", 1);
  test.reverseTable->set("x", 4);
  test.reverseTable->set("y", 6);

  auto result1 = test.handler.queryModifies({StmtType::None, 0},
                                            {EntityType::None, ""});

  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1, 4, 6}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals
              == pair_set<int, string>({{1, "x"}, {1, "z"}, {4, "x"},
                                        {6, "y"}}));
}

/** Modifies(EntityRef, EntityRef) */
// Both args known
TEST_CASE("ModifiesQueryHandler Modifies(procedureName, variableName)") {
  auto test = modifiesTestInit();
  test.table->set(1, "x");
  test.table->set(2, "y");
  test.table->set(3, "z");

  test.structureProvider->procedureToStmtNum.set("main", 1);
  test.structureProvider->procedureToStmtNum.set("main", 2);
  test.structureProvider->procedureToStmtNum.set("foo", 3);
  test.structureProvider->stmtNumToProcedure.set(1, "main");
  test.structureProvider->stmtNumToProcedure.set(2, "main");
  test.structureProvider->stmtNumToProcedure.set(3, "foo");

  auto result1 = test.handler.queryModifies({EntityType::Procedure, "main"},
                                            {EntityType::None, "x"});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals == pair_set<string, string>({{"main", "x"}}));

  auto result2 = test.handler.queryModifies({EntityType::Procedure, "foo"},
                                            {EntityType::None, "x"});
  REQUIRE(result2.isEmpty == true);

  // invalid type
  auto result3 = test.handler.queryModifies({EntityType::None, "main"},
                                            {EntityType::None, "x"});
  REQUIRE(result3.isEmpty == true);
}

// Only arg1 known
TEST_CASE("ModifiesQueryHandler Modifies(procedureName, type)") {
  auto test = modifiesTestInit();
  test.table->set(1, "x");
  test.table->set(2, "y");
  test.table->set(2, "w");
  test.table->set(3, "z");

  test.structureProvider->procedureToStmtNum.set("main", 1);
  test.structureProvider->procedureToStmtNum.set("main", 2);
  test.structureProvider->procedureToStmtNum.set("foo", 3);
  test.structureProvider->stmtNumToProcedure.set(1, "main");
  test.structureProvider->stmtNumToProcedure.set(2, "main");
  test.structureProvider->stmtNumToProcedure.set(3, "foo");

  auto result1 = test.handler.queryModifies({EntityType::Procedure, "main"},
                                            {EntityType::Variable, ""});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "w"}));
  REQUIRE(result1.pairVals
              == pair_set<string, string>({{"main", "x"}, {"main", "y"},
                                           {"main", "w"}}));

  auto result2 = test.handler.queryModifies({EntityType::Procedure, "goo"},
                                            {EntityType::Variable, ""});
  REQUIRE(result2.isEmpty == true);

  // invalid arg1
  auto result3 = test.handler.queryModifies({EntityType::None, "main"},
                                            {EntityType::None, ""});
  REQUIRE(result3.isEmpty == true);
}

// Only arg2 known
TEST_CASE("ModifiesQueryHandler Modifies(type, variable)") {
  auto test = modifiesTestInit();
  test.reverseTable->set("x", 1);
  test.reverseTable->set("x", 3);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("y", 4);

  test.structureProvider->procedureToStmtNum.set("main", 1);
  test.structureProvider->procedureToStmtNum.set("main", 2);
  test.structureProvider->procedureToStmtNum.set("foo", 3);
  test.structureProvider->procedureToStmtNum.set("goo", 4);
  test.structureProvider->stmtNumToProcedure.set(1, "main");
  test.structureProvider->stmtNumToProcedure.set(2, "main");
  test.structureProvider->stmtNumToProcedure.set(3, "foo");
  test.structureProvider->stmtNumToProcedure.set(4, "goo");

  auto result1 = test.handler.queryModifies({EntityType::Procedure, ""},
                                            {EntityType::Variable, "x"});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x"}));
  REQUIRE(result1.pairVals
              == pair_set<string, string>({{"main", "x"}, {"foo", "x"}}));

  // invalid arg1
  auto result2 = test.handler.queryModifies({EntityType::None, ""},
                                            {EntityType::None, "y"});
  REQUIRE(result2.isEmpty == true);
}

// Both args unknown
TEST_CASE("ModifiesQueryHandler Modifies(type, type)") {
  auto test = modifiesTestInit();
  test.reverseTable->set("x", 1);
  test.reverseTable->set("z", 3);
  test.reverseTable->set("y", 2);
  test.reverseTable->set("y", 3);

  test.structureProvider->procedureToStmtNum.set("main", 1);
  test.structureProvider->procedureToStmtNum.set("main", 2);
  test.structureProvider->procedureToStmtNum.set("foo", 3);
  test.structureProvider->procedureToStmtNum.set("goo", 4);
  test.structureProvider->stmtNumToProcedure.set(1, "main");
  test.structureProvider->stmtNumToProcedure.set(2, "main");
  test.structureProvider->stmtNumToProcedure.set(3, "foo");
  test.structureProvider->stmtNumToProcedure.set(4, "goo");

  auto result1 = test.handler.queryModifies({EntityType::Procedure, ""},
                                            {EntityType::None, ""});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>({"main", "foo"}));
  REQUIRE(result1.secondArgVals == unordered_set<string>({"x", "y", "z"}));
  REQUIRE(result1.pairVals
              == pair_set<string, string>({{"main", "x"}, {"main", "y"},
                                           {"foo", "z"}, {"foo", "y"}}));

  // invalid arg1
  auto result2 = test.handler.queryModifies({EntityType::None, ""},
                                            {EntityType::Variable, ""});
  REQUIRE(result2.isEmpty == true);
}
