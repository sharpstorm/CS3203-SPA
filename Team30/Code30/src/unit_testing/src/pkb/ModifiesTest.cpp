#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

TEST_CASE("Modifies (StmtRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addModifies(3, "x", "main");
  writer.addModifies(4, "x", "main");
  writer.addModifies(4, "y", "main");
  writer.addModifies(1, "x", "main");
  writer.addModifies(1, "y", "main");
  writer.addModifies(2, "x", "main");
  writer.addModifies(2, "y", "main");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Assign);
  writer.addVariable("x");
  writer.addVariable("y");
  writer.addVariable("z");

  auto result1 =
      *handler.queryModifies({StmtType::None, 1}, {EntityType::None, "y"});
  REQUIRE(result1.empty() == false);

  auto result2 =
      *handler.queryModifies({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result2.getRightVals() == EntityValueSet({"y", "x"}));
  auto result3 =
      *handler.queryModifies({StmtType::Assign, 0}, {EntityType::None, "x"});
  REQUIRE(result3.getLeftVals() == StmtValueSet({4}));
  auto result4 =
      *handler.queryModifies({StmtType::None, 0}, {EntityType::Variable, ""});
  REQUIRE(result4.getPairVals() ==
          pair_set<StmtValue, EntityValue>({{1, "x"},
                                            {1, "y"},
                                            {2, "x"},
                                            {2, "y"},
                                            {3, "x"},
                                            {4, "x"},
                                            {4, "y"}}));

  auto result5 =
      *handler.queryModifies({StmtType::None, 1}, {EntityType::Wildcard, ""});
  REQUIRE(result5.empty() == false);

  auto result6 =
      *handler.queryModifies({StmtType::None, 0}, {EntityType::Wildcard, ""});
  REQUIRE(result6.empty() == false);
  REQUIRE(result6.getLeftVals() == StmtValueSet({1, 2, 3, 4}));
}

TEST_CASE("Modifies getReadDeclarations") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addModifies(1, "x", "main");
  writer.addModifies(2, "y", "main");
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addVariable("x");
  writer.addVariable("y");

  REQUIRE(handler.getReadDeclarations(2) == "y");
}

TEST_CASE("Modifies (EntityRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addModifies(1, "x", "main");
  writer.addModifies(1, "y", "main");
  writer.addModifies(3, "z", "main");
  writer.addModifies(4, "w", "foo");
  writer.addModifies(5, "z", "goo");
  writer.addVariable("x");
  writer.addVariable("y");
  writer.addVariable("z");
  writer.addVariable("w");
  writer.addVariable("a");
  writer.addProcedure("main", 1, 3);
  writer.addProcedure("foo", 4, 4);
  writer.addProcedure("goo", 5, 5);
  writer.addProcedure("hoo", 6, 6);

  auto result1 = *handler.queryModifies({EntityType::Procedure, "foo"},
                                        {EntityType::None, "w"});
  REQUIRE(result1.empty() == false);

  auto result2 = *handler.queryModifies({EntityType::Procedure, "main"},
                                        {EntityType::Variable, ""});
  REQUIRE(result2.getRightVals() == EntityValueSet({"x", "y", "z"}));
  auto result3 = *handler.queryModifies({EntityType::Procedure, ""},
                                        {EntityType::None, "z"});
  REQUIRE(result3.getLeftVals() == EntityValueSet({"main", "goo"}));

  auto result4 = *handler.queryModifies({EntityType::Procedure, ""},
                                        {EntityType::Variable, ""});
  REQUIRE(result4.getPairVals() ==
          pair_set<EntityValue, EntityValue>({{"main", "x"},
                                              {"main", "y"},
                                              {"main", "z"},
                                              {"foo", "w"},
                                              {"goo", "z"}}));
  auto result5 = *handler.queryModifies({EntityType::Procedure, ""},
                                        {EntityType::Wildcard, ""});
  REQUIRE(result5.getLeftVals() == EntityValueSet({"main", "foo", "goo"}));
  auto result6 = *handler.queryModifies({EntityType::Procedure, "foo"},
                                        {EntityType::Wildcard, ""});
  REQUIRE(result6.empty() == false);
}
