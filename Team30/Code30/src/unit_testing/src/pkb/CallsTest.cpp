#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/EntityMappingProvider.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

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

TEST_CASE("Calls & CallsStar wildcard") {
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

  auto result2 = *handler.queryCalls({EntityType::Procedure, "main"},
                                     {EntityType::Wildcard, ""});
  REQUIRE(result2.isEmpty == false);

  auto result3 = *handler.queryCallsStar({EntityType::Wildcard, ""},
                                         {EntityType::Procedure, "goo"});
  REQUIRE(result3.isEmpty == false);

  auto result4 = *handler.queryCallsStar({EntityType::Procedure, "main"},
                                         {EntityType::Wildcard, ""});
  REQUIRE(result4.isEmpty == false);
  auto result5 = *handler.queryCallsStar({EntityType::Wildcard, ""},
                                         {EntityType::Procedure, "main"});
  REQUIRE(result5.isEmpty == true);

  auto result6 = *handler.queryCallsStar({EntityType::Procedure, "goo"},
                                         {EntityType::Wildcard, ""});
  REQUIRE(result6.isEmpty == true);
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
}

TEST_CASE("Calls & CallsStar 2 unknowns no results") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addProcedure("main", 1, 2);
  writer.addProcedure("foo", 3, 4);
  writer.addProcedure("woo", 5, 6);
  writer.addProcedure("goo", 7, 8);
  writer.runPostProcessor();

  auto result1 = *handler.queryCalls({EntityType::Wildcard, ""},
                                     {EntityType::Wildcard, ""});
  REQUIRE(result1.isEmpty == true);
}
