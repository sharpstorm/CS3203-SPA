#include "catch.hpp"
#include "common/Types.h"
#include "pkb/errors/PKBError.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

TEST_CASE("PostProcessWriter cyclic calls throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addProcedure("main", 1, 1);
  writer.addCalls(1, "main", "main");

  REQUIRE_THROWS_AS(writer.runPostProcessor(), PKBError);
}

TEST_CASE("PostProcessWriter non-existent procedure throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addProcedure("main", 1, 1);

  REQUIRE_THROWS_AS(writer.runPostProcessor(), PKBError);
}

TEST_CASE("PostProcessWriter post process modifies relation") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addStatement(1, StmtType::Call);
  writer.addModifies(2, "x", "foo");
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);

  writer.runPostProcessor();

  auto result1 = pkb->modifiesTable->get(1);
  REQUIRE(result1 == EntityValueSet({"x"}));

  auto result2 = pkb->modifiesPTable->get("main");
  REQUIRE(result2 == EntityValueSet({"x"}));
}

TEST_CASE("PostProcessWriter post process uses relation") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "foo");
  writer.addStatement(1, StmtType::Call);
  writer.addUses(2, "x", "foo");
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);

  writer.runPostProcessor();

  auto result1 = pkb->usesTable->get(1);
  REQUIRE(result1 == EntityValueSet({"x"}));

  auto result2 = pkb->usesPTable->get("main");
  REQUIRE(result2 == EntityValueSet({"x"}));
}

TEST_CASE("PostProcessWriter post process parentT relation") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addProcedure("main", 1, 4);
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Assign);
  writer.addStatement(4, StmtType::Read);
  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(2, 4);

  writer.runPostProcessor();

  auto result1 = pkb->parentTStorage->getByFirstArg(1);
  REQUIRE(result1 == 4);
  auto result2 = pkb->parentTStorage->getByFirstArg(2);
  REQUIRE(result2 == 4);

  auto result3 = pkb->parentTStorage->getBySecondArg(4);
  REQUIRE(result3 == StmtValueSet({1, 2}));
  auto result4 = pkb->parentTStorage->getBySecondArg(3);
  REQUIRE(result4 == StmtValueSet({1, 2}));
  auto result5 = pkb->parentTStorage->getBySecondArg(2);
  REQUIRE(result5 == StmtValueSet({1}));
}

TEST_CASE("PostProcessWriter post process callsT relation") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addProcedure("main", 1, 1);
  writer.addProcedure("foo", 2, 2);
  writer.addProcedure("example", 3, 3);
  writer.addCalls(1, "main", "foo");
  writer.addCalls(2, "foo", "example");

  writer.runPostProcessor();

  auto result1 = pkb->callsTTable->get("main");
  REQUIRE(result1 == EntityValueSet({"foo", "example"}));
  auto result2 = pkb->callsTTable->get("foo");
  REQUIRE(result2 == EntityValueSet({"example"}));
  auto result3 = pkb->callsTTable->get("example");
  REQUIRE(result3 == EntityValueSet({}));

  auto result4 = pkb->callsTRevTable->get("example");
  REQUIRE(result4 == EntityValueSet({"main", "foo"}));
  auto result5 = pkb->callsTRevTable->get("foo");
  REQUIRE(result5 == EntityValueSet({"main"}));
  auto result6 = pkb->callsTRevTable->get("main");
  REQUIRE(result6 == EntityValueSet({}));
}
