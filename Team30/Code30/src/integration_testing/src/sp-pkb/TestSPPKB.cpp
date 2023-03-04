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
#include "common/ast/math/math_operand/PlusASTNode.h"
#include "common/ast/entity/VariableASTNode.h"

using std::make_unique, std::make_shared, std::unordered_set, std::to_string;

string testSource = "procedure average {"
                    "  read num1;"
                    "  sum = num1 + num2 + num3;"
                    "  while (a < b) {"
                    "    abc = 3;"
                    "  }"
                    "  read abc;"
                    "}";

string testSource2 = "procedure average {"
                    "  read num1;"
                    "  sum = num1 + num2 + num3;"
                    "  while (a < b) {"
                    "    abc = 3;"
                    "  }"
                    "  read abc;"
                    "}"
                    "procedure main {"
                    "  call a2;"
                    "}"
                    "procedure a2 {"
                    "  flag = 0;"
                    "  call average;"
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
  REQUIRE(queryHandler.getSymbolsOfType(EntityType::Variable) ==
      unordered_set<string>({"num1", "num2", "num3", "sum", "a", "b", "abc"}));
}

TEST_CASE("Test Write Calls") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);
  REQUIRE(!queryHandler.queryCalls(EntityRef{EntityType::Procedure, "a2"},
                                   EntityRef{EntityType::Procedure, "average"}).isEmpty);
  REQUIRE(!queryHandler.queryCalls(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"}).isEmpty);
}

TEST_CASE("Test Write Calls star") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "a2"},
                                   EntityRef{EntityType::Procedure, "average"}).isEmpty);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"}).isEmpty);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "average"}).isEmpty);
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

TEST_CASE("Test ModifiesP") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "sum"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "abc"}).isEmpty);

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "a2"},
                                      EntityRef{EntityType::Variable, "flag"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "a2"},
                                      EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "a2"},
                                      EntityRef{EntityType::Variable, "sum"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "a2"},
                                      EntityRef{EntityType::Variable, "abc"}).isEmpty);

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "main"},
                                      EntityRef{EntityType::Variable, "flag"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "main"},
                                      EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "main"},
                                      EntityRef{EntityType::Variable, "sum"}).isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::Procedure, "main"},
                                      EntityRef{EntityType::Variable, "abc"}).isEmpty);
}

TEST_CASE("Test UsesP") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "num2"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "average"},
                                      EntityRef{EntityType::Variable, "num3"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "average"},
                                  EntityRef{EntityType::Variable, "a"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "average"},
                                  EntityRef{EntityType::Variable, "b"}).isEmpty);

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "a2"},
                                  EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "a2"},
                                  EntityRef{EntityType::Variable, "num2"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "a2"},
                                  EntityRef{EntityType::Variable, "num3"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "a2"},
                                  EntityRef{EntityType::Variable, "a"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "a2"},
                                  EntityRef{EntityType::Variable, "b"}).isEmpty);


  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "main"},
                                  EntityRef{EntityType::Variable, "num1"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "main"},
                                  EntityRef{EntityType::Variable, "num2"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "main"},
                                  EntityRef{EntityType::Variable, "num3"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "main"},
                                  EntityRef{EntityType::Variable, "a"}).isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::Procedure, "main"},
                                  EntityRef{EntityType::Variable, "b"}).isEmpty);
}
