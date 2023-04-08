#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

TEST_CASE("Uses (StmtRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addUses(3, "x", "main");
  writer.addUses(4, "x", "main");
  writer.addUses(4, "y", "main");
  writer.addUses(1, "x", "main");
  writer.addUses(1, "y", "main");
  writer.addUses(2, "x", "main");
  writer.addUses(2, "y", "main");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Print);
  writer.addStatement(4, StmtType::Assign);
  writer.addVariable("x");
  writer.addVariable("y");
  writer.addVariable("z");

  auto result1 =
      *handler.queryUses({StmtType::None, 1}, {EntityType::None, "y"});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      *handler.queryUses({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result2.secondArgVals == EntityValueSet({"y", "x"}));

  auto result3 =
      *handler.queryUses({StmtType::Assign, 0}, {EntityType::None, "x"});
  REQUIRE(result3.firstArgVals == StmtValueSet({4}));

  auto result4 =
      *handler.queryUses({StmtType::None, 0}, {EntityType::Variable, ""});
  REQUIRE(result4.pairVals == pair_set<StmtValue, EntityValue>({
                                  {1, "x"},
                                  {1, "y"},
                                  {2, "x"},
                                  {2, "y"},
                                  {3, "x"},
                                  {4, "x"},
                                  {4, "y"},
                              }));

  auto result5 =
      *handler.queryUses({StmtType::None, 1}, {EntityType::Wildcard, ""});
  REQUIRE(result5.isEmpty == false);

  auto result6 =
      *handler.queryUses({StmtType::None, 0}, {EntityType::Wildcard, ""});
  REQUIRE(result6.isEmpty == false);
  REQUIRE(result6.firstArgVals == StmtValueSet({1, 2, 3, 4}));
  // print Declaration
  REQUIRE(handler.getPrintDeclarations(3) == "x");
}

TEST_CASE("Uses (EntityRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addUses(1, "x", "main");
  writer.addUses(1, "y", "main");
  writer.addUses(3, "z", "main");
  writer.addUses(4, "w", "foo");
  writer.addUses(5, "z", "goo");
  writer.addVariable("x");
  writer.addVariable("y");
  writer.addVariable("z");
  writer.addVariable("w");
  writer.addVariable("a");
  writer.addProcedure("main", 1, 3);
  writer.addProcedure("foo", 4, 4);
  writer.addProcedure("goo", 5, 5);
  writer.addProcedure("hoo", 6, 6);

  auto result1 = *handler.queryUses({EntityType::Procedure, "foo"},
                                    {EntityType::None, "w"});
  REQUIRE(result1.isEmpty == false);
  auto result2 = *handler.queryUses({EntityType::Procedure, "main"},
                                    {EntityType::Variable, ""});
  REQUIRE(result2.secondArgVals == EntityValueSet({"x", "y", "z"}));
  auto result3 =
      *handler.queryUses({EntityType::Procedure, ""}, {EntityType::None, "z"});
  REQUIRE(result3.firstArgVals == EntityValueSet({"main", "goo"}));
  auto result4 = *handler.queryUses({EntityType::Procedure, ""},
                                    {EntityType::Variable, ""});
  REQUIRE(result4.pairVals ==
          pair_set<EntityValue, EntityValue>({{"main", "x"},
                                              {"main", "y"},
                                              {"main", "z"},
                                              {"foo", "w"},
                                              {"goo", "z"}}));
  auto result5 = *handler.queryUses({EntityType::Procedure, ""},
                                    {EntityType::Wildcard, ""});
  REQUIRE(result5.firstArgVals == EntityValueSet({"main", "foo", "goo"}));
  auto result6 = *handler.queryUses({EntityType::Procedure, "foo"},
                                    {EntityType::Wildcard, ""});
  REQUIRE(result6.isEmpty == false);
}
