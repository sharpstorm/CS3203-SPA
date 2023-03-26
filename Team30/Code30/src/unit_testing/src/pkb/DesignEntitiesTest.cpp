#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("Entities write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());

  writer.addVariable("a");
  writer.addVariable("b");
  writer.addVariable("c");
  writer.addConstant("0");
  writer.addConstant("1");
  writer.addProcedure("main", 3, 4);

  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable) ==
          unordered_set<string>({"a", "b", "c"}));
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Constant) ==
          unordered_set<string>({"0", "1"}));

  REQUIRE(queryHandler.isSymbolOfType(EntityType::Constant, "0") == true);
  REQUIRE(queryHandler.isSymbolOfType(EntityType::Variable, "a") == true);
  REQUIRE(queryHandler.isSymbolOfType(EntityType::Procedure, "main") == true);
  REQUIRE(queryHandler.isSymbolOfType(EntityType::None, "main") == false);

  REQUIRE(queryHandler.getVariableByIndex(1) == "a");
  REQUIRE(queryHandler.getConstantByIndex(1) == "0");

  REQUIRE(queryHandler.getIndexOfVariable("b") == 2);
  REQUIRE(queryHandler.getIndexOfConstant("1") == 2);
}

TEST_CASE("Statements write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Assign);
  writer.addStatement(3, StmtType::Assign);
  writer.addStatement(4, StmtType::Read);
  writer.addStatement(5, StmtType::Call);
  writer.addCalls(5, "main", "sub");

  REQUIRE(queryHandler.getStatementsOfType(StmtType::Assign) ==
          unordered_set<int>({1, 2, 3}));
  REQUIRE(queryHandler.getStatementsOfType(StmtType::Read) ==
          unordered_set<int>({4}));
  // get all statement_types
  REQUIRE(queryHandler.getStatementsOfType(StmtType::None) ==
          unordered_set<int>({1, 2, 3, 4, 5}));
  REQUIRE(queryHandler.getStatementType(1) == StmtType::Assign);
  REQUIRE(queryHandler.isStatementOfType(StmtType::Assign, 1) == true);
  REQUIRE(queryHandler.isStatementOfType(StmtType::Read, 1) == false);

  REQUIRE(queryHandler.getCalledDeclaration(5) == "sub");
}
