#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "EntityMappingProviderStub.h"
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/CallsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"

using std::make_shared;
using std::make_unique;
using std::pair;
using std::string;
using std::unique_ptr;
using std::unordered_set;

static unique_ptr<EntityMappingProviderStub> setUpEntityMappingProvider() {
  auto provider = make_unique<EntityMappingProviderStub>();
  provider->procedureTable.insert(1, "main");
  provider->procedureTable.insert(2, "foo");
  provider->procedureTable.insert(3, "woo");
  provider->procedureTable.insert(4, "goo");
  provider->allProcedures = {"main", "foo", "woo", "goo"};
  return provider;
}

struct callsTest {
  shared_ptr<CallsTable> table = make_shared<CallsTable>();
  shared_ptr<CallsRevTable> reverseTable = make_shared<CallsRevTable>();
  unique_ptr<CallsStorage> store =
      make_unique<CallsStorage>(table.get(), reverseTable.get());
  shared_ptr<CallsTTable> tTable = make_shared<CallsTTable>();
  shared_ptr<CallsTRevTable> tReverseTable = make_shared<CallsTRevTable>();
  unique_ptr<CallsTStorage> storeT =
      make_unique<CallsTStorage>(tTable.get(), tReverseTable.get());
  unique_ptr<EntityMappingProviderStub> entityProvider =
      setUpEntityMappingProvider();
  unique_ptr<EntityPredicateFactory> factory =
      make_unique<EntityPredicateFactory>();
  unique_ptr<PkbEntEntQueryInvoker> invoker =
      make_unique<PkbEntEntQueryInvoker>(entityProvider.get(), factory.get());
  CallsQueryHandler handler = CallsQueryHandler(invoker.get(), store.get());
  CallsQueryHandler handlerT = CallsQueryHandler(invoker.get(), storeT.get());

  QueryResultPtr<EntityValue, EntityValue> query(EntityRef leftArg,
                                                 EntityRef rightArg) {
    return handler.query(&leftArg, &rightArg);
  }

  QueryResultPtr<EntityValue, EntityValue> queryT(EntityRef leftArg,
                                                  EntityRef rightArg) {
    return handlerT.query(&leftArg, &rightArg);
  }
};

/* Calls */

TEST_CASE("CallsQueryHandler calls(entityName, entityName)") {
  auto test = callsTest();

  test.table->insert("main", "foo");
  test.table->insert("foo", "goo");

  REQUIRE(test.query({EntityType::Procedure, "main"},
                     {EntityType::Procedure, "foo"})
              .get()
              ->isEmpty == false);
  REQUIRE(
      test.query({EntityType::Procedure, "foo"}, {EntityType::Procedure, "goo"})
          .get()
          ->isEmpty == false);
  REQUIRE(test.query({EntityType::Procedure, "main"},
                     {EntityType::Procedure, "goo"})
              .get()
              ->isEmpty == true);
}

TEST_CASE("CallsQueryHandler calls(_, entityName)") {
  auto test = callsTest();

  test.table->insert("main", "foo");
  test.table->insert("foo", "goo");
  test.reverseTable->insert("foo", "main");

  auto result =
      *test.query({EntityType::Wildcard, ""}, {EntityType::Procedure, "foo"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"foo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"}});
}

TEST_CASE("CallsQueryHandler calls(entityName, _)") {
  auto test = callsTest();

  test.table->insert("main", "foo");
  test.table->insert("main", "woo");
  test.table->insert("foo", "goo");

  auto result =
      *test.query({EntityType::Procedure, "main"}, {EntityType::Wildcard, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo"});
  REQUIRE(result.pairVals ==
          pair_set<string, string>{{"main", "foo"}, {"main", "woo"}});
}

TEST_CASE("CallsQueryHandler calls(_, _)") {
  auto test = callsTest();

  test.table->insert("main", "foo");
  test.table->insert("main", "woo");
  test.table->insert("foo", "goo");

  auto result =
      *test.query({EntityType::Wildcard, ""}, {EntityType::Wildcard, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"foo", "goo"}});
}

/* Call Star */
TEST_CASE("CallsQueryHandler callsStar(entityName, entityName)") {
  auto test = callsTest();

  test.tTable->insert("main", "foo");
  test.tTable->insert("foo", "goo");
  test.tTable->insert("main", "goo");

  REQUIRE(test.queryT({EntityType::Procedure, "main"},
                      {EntityType::Procedure, "foo"})
              .get()
              ->isEmpty == false);
  REQUIRE(test.queryT({EntityType::Procedure, "foo"},
                      {EntityType::Procedure, "goo"})
              .get()
              ->isEmpty == false);
  REQUIRE(test.queryT({EntityType::Procedure, "main"},
                      {EntityType::Procedure, "goo"})
              .get()
              ->isEmpty == false);
}

TEST_CASE("CallsQueryHandler callsStar(_, entityName)") {
  auto test = callsTest();

  test.tTable->insert("main", "foo");
  test.tTable->insert("foo", "goo");
  test.tTable->insert("main", "goo");
  test.tReverseTable->insert("foo", "main");
  test.tReverseTable->insert("goo", "foo");
  test.tReverseTable->insert("goo", "main");

  auto result =
      *test.queryT({EntityType::Wildcard, ""}, {EntityType::Procedure, "goo"});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"foo", "main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"goo"});
  REQUIRE(result.pairVals ==
          pair_set<string, string>{{"main", "goo"}, {"foo", "goo"}});
}

TEST_CASE("CallsQueryHandler callsStar(entityName, _)") {
  auto test = callsTest();

  test.tTable->insert("main", "foo");
  test.tTable->insert("main", "woo");
  test.tTable->insert("foo", "goo");
  test.tTable->insert("main", "goo");

  auto result =
      *test.queryT({EntityType::Procedure, "main"}, {EntityType::Wildcard, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"main", "goo"}});
}

TEST_CASE("CallsQueryHandler callsStar(_, _)") {
  auto test = callsTest();

  test.tTable->insert("main", "foo");
  test.tTable->insert("main", "woo");
  test.tTable->insert("foo", "goo");
  test.tTable->insert("main", "goo");

  auto result =
      *test.queryT({EntityType::Wildcard, ""}, {EntityType::Wildcard, ""});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result.pairVals == pair_set<string, string>{{"main", "foo"},
                                                      {"main", "woo"},
                                                      {"main", "goo"},
                                                      {"foo", "goo"}});
}
