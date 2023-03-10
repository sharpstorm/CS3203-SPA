#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/errors/PKBError.h"

using std::string, std::unordered_set;

TEST_CASE("PostProcessWriter cyclic calls throws error") {
  unique_ptr<PKB> pkb = std::make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  writer.addCalls(1, "main", "main");
  writer.addProcedure("main", 1, 1);

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
  REQUIRE(result1 == unordered_set<string>({"x"}));

  auto result2 = pkb->modifiesPStorage->getByFirstArg("main");
  REQUIRE(result2 == unordered_set<string>({"x"}));
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
  REQUIRE(result1 == unordered_set<string>({"x"}));

  auto result2 = pkb->usesPStorage->getByFirstArg("main");
  REQUIRE(result2 == unordered_set<string>({"x"}));
}
