#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/errors/PKBError.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::string, std::unordered_set;

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

  auto result1 = pkb->modifiesStorage->getByFirstArg(1);
  REQUIRE(result1 == set<string>({"x"}));

  auto result2 = pkb->modifiesPStorage->getByFirstArg("main");
  REQUIRE(result2 == set<string>({"x"}));
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

  auto result1 = pkb->usesStorage->getByFirstArg(1);
  REQUIRE(result1 == set<string>({"x"}));

  auto result2 = pkb->usesPStorage->getByFirstArg("main");
  REQUIRE(result2 == set<string>({"x"}));
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
  REQUIRE(result3 == StmtSet({1, 2}));
  auto result4 = pkb->parentTStorage->getBySecondArg(3);
  REQUIRE(result4 == StmtSet({1, 2}));
  auto result5 = pkb->parentTStorage->getBySecondArg(2);
  REQUIRE(result5 == StmtSet({1}));
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

  auto result1 = pkb->callsTStorage->getByFirstArg("main");
  REQUIRE(result1 == EntitySet({"foo", "example"}));
  auto result2 = pkb->callsTStorage->getByFirstArg("foo");
  REQUIRE(result2 == EntitySet({"example"}));
  auto result3 = pkb->callsTStorage->getByFirstArg("example");
  REQUIRE(result3 == EntitySet({}));

  auto result4 = pkb->callsTStorage->getBySecondArg("example");
  REQUIRE(result4 == EntitySet({"main", "foo"}));
  auto result5 = pkb->callsTStorage->getBySecondArg("foo");
  REQUIRE(result5 == EntitySet({"main"}));
  auto result6 = pkb->callsTStorage->getBySecondArg("main");
  REQUIRE(result6 == EntitySet({}));
}
