#include "catch.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <unordered_set>
#include "qps/QPSFacade.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/errors/SPError.h"
#include "sp/SourceParser.h"
#include "sp/SpDriver.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

string testSource = "procedure average {"
                    "  read num1;"
                    "  sum = num1 + num2 + num3;"
                    "  while (a < b) {"
                    "    abc = 3;"
                    "  }"
                    "  read abc;"
                    "}";


TEST_CASE("Test Write Follows") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 1}, StmtRef{StmtType::None, 2}).isEmpty);
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 2}, StmtRef{StmtType::None, 3}).isEmpty);
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 5}).isEmpty);
}

TEST_CASE("Test Write Parent") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryParent(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 4}).isEmpty);
}

TEST_CASE("Test Write Uses") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num2"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num3"}).isEmpty);
}

TEST_CASE("Test Write Modifies") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "sum"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 4}, EntityRef{EntityType::Variable, "abc"}).isEmpty);
}

TEST_CASE("Test Write Entity") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable).size() == 7);
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable) ==
      unordered_set<string>({"num1", "num2", "num3", "sum", "a", "b", "abc"}));
}

TEST_CASE("Test Writer Pattern") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  shared_ptr<IASTNode> node = *queryHandler.
          queryAssigns(StmtRef{StmtType::None, 2})
      .secondArgVals.begin();
  PlusASTNode plus;
  VariableASTNode v1("num1");
  VariableASTNode v2("num2");
  VariableASTNode v3("num3");
  REQUIRE(node->isEquals(&plus));
  REQUIRE(node->getChild(0)->isEquals(&plus));
  REQUIRE(node->getChild(1)->isEquals(&v3));
  REQUIRE(node->getChild(0)->getChild(0)->isEquals(&v1));
  REQUIRE(node->getChild(0)->getChild(1)->isEquals(&v2));
}

TEST_CASE("Test Bad Program") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  REQUIRE_THROWS_AS(spDriver.parseSource("", &pkbWriter), SPError);
  REQUIRE_THROWS_AS(spDriver.parseSource("procedure xxx{}", &pkbWriter), SPError);
  REQUIRE_THROWS_AS(spDriver.parseSource("procedure xxx{if(x==1)then{x=1;}else{}}", &pkbWriter), SPError);
  REQUIRE_THROWS_AS(spDriver.parseSource("procedure xxx{while(x==1){}}", &pkbWriter), SPError);
}
