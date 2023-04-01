#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/EntityMappingProvider.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;
using std::unordered_set;

TEST_CASE("Calls & callsStar 2 knowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addProcedure("main", 1, 3);
  writer.addProcedure("foo", 4, 4);
  writer.addProcedure("goo", 5, 5);
  writer.addProcedure("woo", 6, 6);
  writer.addCalls(2, "main", "foo");
  writer.addCalls(3, "main", "woo");
  writer.addCalls(4, "foo", "goo");
  writer.runPostProcessor();

  auto result1 = *handler.queryCalls({EntityType::Procedure, "main"},
                                     {EntityType::Procedure, "foo"});
  REQUIRE(result1.isEmpty == false);

  auto result2 = *handler.queryCalls({EntityType::Procedure, "main"},
                                     {EntityType::Procedure, "goo"});
  REQUIRE(result2.isEmpty == true);

  auto result3 = *handler.queryCallsStar({EntityType::Procedure, "main"},
                                         {EntityType::Procedure, "foo"});
  REQUIRE(result3.isEmpty == false);

  auto result4 = *handler.queryCallsStar({EntityType::Procedure, "main"},
                                         {EntityType::Procedure, "goo"});
  REQUIRE(result4.isEmpty == false);
}

TEST_CASE("Calls & CallsStar 1 unknown") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addProcedure("main", 1, 3);
  writer.addProcedure("foo", 4, 4);
  writer.addProcedure("goo", 5, 5);
  writer.addProcedure("woo", 6, 6);
  writer.addCalls(2, "main", "foo");
  writer.addCalls(3, "main", "woo");
  writer.addCalls(4, "foo", "goo");
  writer.runPostProcessor();

  auto result1 = *handler.queryCalls({EntityType::Wildcard, ""},
                                     {EntityType::Procedure, "foo"});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result1.secondArgVals == unordered_set<string>{"foo"});
  REQUIRE(result1.pairVals == pair_set<string, string>{{"main", "foo"}});

  auto result2 = *handler.queryCalls({EntityType::Procedure, "main"},
                                     {EntityType::Wildcard, ""});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result2.secondArgVals == unordered_set<string>{"woo", "foo"});
  REQUIRE(result2.pairVals ==
          pair_set<string, string>{{"main", "foo"}, {"main", "woo"}});

  auto result3 = *handler.queryCallsStar({EntityType::Wildcard, ""},
                                         {EntityType::Procedure, "goo"});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.firstArgVals == unordered_set<string>{"foo", "main"});
  REQUIRE(result3.secondArgVals == unordered_set<string>{"goo"});
  REQUIRE(result3.pairVals ==
          pair_set<string, string>{{"main", "goo"}, {"foo", "goo"}});

  auto result4 = *handler.queryCallsStar({EntityType::Procedure, "main"},
                                         {EntityType::Wildcard, ""});
  REQUIRE(result4.isEmpty == false);
  REQUIRE(result4.firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result4.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result4.pairVals == pair_set<string, string>{{"main", "foo"},
                                                       {"main", "woo"},
                                                       {"main", "goo"}});
}

TEST_CASE("Calls & CallsStar 2 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addCalls(2, "main", "foo");
  writer.addCalls(3, "main", "woo");
  writer.addCalls(4, "foo", "goo");
  writer.addProcedure("main", 1, 2);
  writer.addProcedure("foo", 3, 4);
  writer.addProcedure("woo", 5, 6);
  writer.addProcedure("goo", 7, 8);
  writer.runPostProcessor();

  auto result1 = *handler.queryCalls({EntityType::Wildcard, ""},
                                     {EntityType::Wildcard, ""});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result1.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result1.pairVals == pair_set<string, string>{{"main", "foo"},
                                                       {"main", "woo"},
                                                       {"foo", "goo"}});

  auto result2 = *handler.queryCallsStar({EntityType::Wildcard, ""},
                                         {EntityType::Wildcard, ""});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result2.secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result2.pairVals == pair_set<string, string>{{"main", "foo"},
                                                       {"main", "woo"},
                                                       {"main", "goo"},
                                                       {"foo", "goo"}});
}
