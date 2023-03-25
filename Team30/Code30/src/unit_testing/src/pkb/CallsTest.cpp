#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/queryHandlers/CallsQueryHandler.h"
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

  writer.addCalls(2, "main", "foo");
  writer.addCalls(3, "main", "woo");
  writer.addCalls(4, "foo", "goo");

  auto result1 = handler.queryCalls({EntityType::Procedure, "main"},
                                    {EntityType::Procedure, "foo"});
  REQUIRE(result1.get()->isEmpty == false);

  auto result2 = handler.queryCalls({EntityType::Procedure, "main"},
                                    {EntityType::Procedure, "goo"});
  REQUIRE(result2.get()->isEmpty == true);

  auto result3 = handler.queryCallsStar({EntityType::Procedure, "main"},
                                        {EntityType::Procedure, "foo"});
  REQUIRE(result3.get()->isEmpty == false);

  auto result4 = handler.queryCallsStar({EntityType::Procedure, "main"},
                                        {EntityType::Procedure, "goo"});
  REQUIRE(result4.get()->isEmpty == false);
}

TEST_CASE("Calls & CallsStar 1 unknown") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addCalls(2, "main", "foo");
  writer.addCalls(3, "main", "woo");
  writer.addCalls(4, "foo", "goo");

  auto result1 = handler.queryCalls({EntityType::None, ""},
                                    {EntityType::Procedure, "foo"});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>{"foo"});
  REQUIRE(result1.get()->pairVals == pair_set<string, string>{{"main", "foo"}});

  auto result2 = handler.queryCalls({EntityType::Procedure, "main"},
                                    {EntityType::None, ""});
  REQUIRE(result2.get()->isEmpty == false);
  REQUIRE(result2.get()->firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result2.get()->secondArgVals == unordered_set<string>{"woo", "foo"});
  REQUIRE(result2.get()->pairVals ==
          pair_set<string, string>{{"main", "foo"}, {"main", "woo"}});

  auto result3 = handler.queryCallsStar({EntityType::None, ""},
                                        {EntityType::Procedure, "goo"});
  REQUIRE(result3.get()->isEmpty == false);
  REQUIRE(result3.get()->firstArgVals == unordered_set<string>{"foo", "main"});
  REQUIRE(result3.get()->secondArgVals == unordered_set<string>{"goo"});
  REQUIRE(result3.get()->pairVals ==
          pair_set<string, string>{{"main", "goo"}, {"foo", "goo"}});

  auto result4 = handler.queryCallsStar({EntityType::Procedure, "main"},
                                        {EntityType::None, ""});
  REQUIRE(result4.get()->isEmpty == false);
  REQUIRE(result4.get()->firstArgVals == unordered_set<string>{"main"});
  REQUIRE(result4.get()->secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result4.get()->pairVals == pair_set<string, string>{{"main", "foo"},
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

  auto result1 =
      handler.queryCalls({EntityType::None, ""}, {EntityType::None, ""});
  REQUIRE(result1.get()->isEmpty == false);
  REQUIRE(result1.get()->firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result1.get()->secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result1.get()->pairVals == pair_set<string, string>{{"main", "foo"},
                                                       {"main", "woo"},
                                                       {"foo", "goo"}});

  auto result2 =
      handler.queryCallsStar({EntityType::None, ""}, {EntityType::None, ""});
  REQUIRE(result2.get()->isEmpty == false);
  REQUIRE(result2.get()->firstArgVals == unordered_set<string>{"main", "foo"});
  REQUIRE(result2.get()->secondArgVals == unordered_set<string>{"woo", "foo", "goo"});
  REQUIRE(result2.get()->pairVals == pair_set<string, string>{{"main", "foo"},
                                                       {"main", "woo"},
                                                       {"main", "goo"},
                                                       {"foo", "goo"}});
}
