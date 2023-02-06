#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::string;
using std::unordered_set;

TEST_CASE("Entities write and read") {
  PKB* pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);

  writer.addSymbol("a", EntityType::Variable);
  writer.addSymbol("b", EntityType::Variable);
  writer.addSymbol("c", EntityType::Variable);
  writer.addSymbol("0", EntityType::Constant);
  writer.addSymbol("1", EntityType::Constant);

  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable) ==
          unordered_set<string>({"a", "b", "c"}));
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Constant) ==
          unordered_set<string>({"0", "1"}));
}

TEST_CASE("Statements write and read") {
  PKB* pkb = new PKB();
  PkbWriter writer = PkbWriter(pkb);
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb);

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Assign);
  writer.addStatement(3, StmtType::Assign);
  writer.addStatement(4, StmtType::Read);

  REQUIRE(queryHandler.getStatementsOfType(StmtType::Assign) ==
          unordered_set<int>({1, 2, 3}));
  REQUIRE(queryHandler.getStatementsOfType(StmtType::Read) ==
          unordered_set<int>({4}));
}