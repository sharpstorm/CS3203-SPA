#include <string>
#include <unordered_set>
#include <memory>
#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::string;
using std::unordered_set;
using std::make_unique;

TEST_CASE("Entities write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());

  writer.addVariable("a");
  writer.addVariable("b");
  writer.addVariable("c");
  writer.addConstant("0");
  writer.addConstant("1");

  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable) ==
      unordered_set<string>({"a", "b", "c"}));
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Constant) ==
      unordered_set<string>({"0", "1"}));
}

TEST_CASE("Statements write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Assign);
  writer.addStatement(3, StmtType::Assign);
  writer.addStatement(4, StmtType::Read);

  REQUIRE(queryHandler.getStatementsOfType(StmtType::Assign) ==
      unordered_set<int>({1, 2, 3}));
  REQUIRE(queryHandler.getStatementsOfType(StmtType::Read) ==
      unordered_set<int>({4}));
  // get all statement_types
  REQUIRE(queryHandler.getStatementsOfType(StmtType::None) ==
      unordered_set<int>({1, 2, 3, 4}));
}

