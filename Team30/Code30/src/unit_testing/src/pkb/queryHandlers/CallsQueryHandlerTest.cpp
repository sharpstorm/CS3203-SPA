#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "EntityMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/queryHandlers/CallsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/tables/ContiguousSetTable.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::string;
using std::unique_ptr;
using std::unordered_set;

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->entityTypeToValue.set(EntityType::Procedure, "main");
  provider->entityTypeToValue.set(EntityType::Procedure, "foo");
  provider->entityTypeToValue.set(EntityType::Procedure, "woo");
  provider->entityTypeToValue.set(EntityType::Procedure, "goo");

  return provider;
}

struct callsTestInit {
  shared_ptr<HashKeySetTable<string, string>> table;
  shared_ptr<HashKeySetTable<string, string>> reverseTable;
  unique_ptr<CallsStorage> store;
  unique_ptr<EntityMappingProviderStub> entityProvider;
  unique_ptr<PredicateFactory> factory;
  CallsQueryHandler handler;

  callsTestInit()
      : table(make_shared<HashKeySetTable<string, string>>()),
        reverseTable(make_shared<HashKeySetTable<string, string>>()),
        store(make_unique<CallsStorage>(table, reverseTable)),
        entityProvider(setUpEntityMappingProvider()),
        factory(make_unique<PredicateFactory>(nullptr, entityProvider.get())),
        handler(CallsQueryHandler(store.get(), factory.get(),
                                  entityProvider.get())) {}
};

/* Calls */

TEST_CASE("CallsQueryHandler calls(entityName, entityName)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("foo", "goo");

  REQUIRE(test.handler
              .queryCalls({EntityType::Procedure, "main"},
                          {EntityType::Procedure, "foo"})
              .isEmpty == false);
  REQUIRE(test.handler
              .queryCalls({EntityType::Procedure, "foo"},
                          {EntityType::Procedure, "goo"})
              .isEmpty == false);
  REQUIRE(test.handler
              .queryCalls({EntityType::Procedure, "main"},
                          {EntityType::Procedure, "goo"})
              .isEmpty == true);
}

TEST_CASE("CallsQueryHandler calls(_, entityName)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("foo", "goo");
  test.reverseTable->set("foo", "main");

  auto result = test.handler.queryCalls({EntityType::None, ""},
                                        {EntityType::Procedure, "foo"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"foo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"}});
}

TEST_CASE("CallsQueryHandler calls(entityName, _)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("main", "woo");
  test.table->set("foo", "goo");

  auto result = test.handler.queryCalls({EntityType::Procedure, "main"},
                                        {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo"});
  REQUIRE(result.pairVals ==
          pair_set<string, string>{{"main", "foo"}, {"main", "woo"}});
}

TEST_CASE("CallsQueryHandler calls(_, _)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("main", "woo");
  test.table->set("foo", "goo");

  auto result =
      test.handler.queryCalls({EntityType::None, ""}, {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"foo", "goo"}});
}

/* Call Star */
TEST_CASE("CallsQueryHandler callsStar(entityName, entityName)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("foo", "goo");

  REQUIRE(test.handler
              .queryCallsStar({EntityType::Procedure, "main"},
                              {EntityType::Procedure, "foo"})
              .isEmpty == false);
  REQUIRE(test.handler
              .queryCallsStar({EntityType::Procedure, "foo"},
                              {EntityType::Procedure, "goo"})
              .isEmpty == false);
  REQUIRE(test.handler
              .queryCallsStar({EntityType::Procedure, "main"},
                              {EntityType::Procedure, "goo"})
              .isEmpty == false);
}

TEST_CASE("CallsQueryHandler callsStar(_, entityName)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("foo", "goo");
  test.reverseTable->set("foo", "main");
  test.reverseTable->set("goo", "foo");

  auto result = test.handler.queryCallsStar({EntityType::None, ""},
                                            {EntityType::Procedure, "goo"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"foo", "main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"goo"});
  REQUIRE(result.pairVals ==
          pair_set<string, string>{{"main", "goo"}, {"foo", "goo"}});
}

TEST_CASE("CallsQueryHandler callsStar(entityName, _)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("main", "woo");
  test.table->set("foo", "goo");

  auto result = test.handler.queryCallsStar({EntityType::Procedure, "main"},
                                            {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"main", "goo"}});
}

TEST_CASE("CallsQueryHandler callsStar(_, _)") {
  auto test = callsTestInit();

  test.table->set("main", "foo");
  test.table->set("main", "woo");
  test.table->set("foo", "goo");

  auto result = test.handler.queryCallsStar({EntityType::None, ""},
                                            {EntityType::None, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"main", "goo"},
                                                      {"foo", "goo"}});
}
