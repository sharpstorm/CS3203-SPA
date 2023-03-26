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
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 1}, StmtRef{StmtType::None, 2})->isEmpty);
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 2}, StmtRef{StmtType::None, 3})->isEmpty);
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 5})->isEmpty);
}

TEST_CASE("Test Write Parent") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryParent(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 4})->isEmpty);
}

TEST_CASE("Test Write Uses") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num2"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num3"})->isEmpty);
}

TEST_CASE("Test Write Modifies") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "sum"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 4}, EntityRef{EntityType::Variable, "abc"})->isEmpty);
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
                                   EntityRef{EntityType::Procedure, "average"})->isEmpty);
  REQUIRE(!queryHandler.queryCalls(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"})->isEmpty);
}

TEST_CASE("Test Write Calls star") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "a2"},
                                   EntityRef{EntityType::Procedure, "average"})->isEmpty);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"})->isEmpty);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "average"})->isEmpty);
}

TEST_CASE("Test Writer Pattern") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  PatternTrie* trie = *queryHandler.
          queryAssigns(StmtRef{StmtType::None, 2})
      ->secondArgVals.begin();
  ExpressionSequence expected{"num1"};
  REQUIRE(trie->isMatchPartial(&expected));
  expected = {"num2"};
  REQUIRE(trie->isMatchPartial(&expected));
  expected = {"num3"};
  REQUIRE(trie->isMatchPartial(&expected));
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

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "sum"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "abc"})->isEmpty);

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "flag"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "sum"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "abc"})->isEmpty);

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "flag"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "sum"})->isEmpty);
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "abc"})->isEmpty);
}

TEST_CASE("Test UsesP") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num2"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num3"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                  EntityRef{EntityType::None, "a"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                  EntityRef{EntityType::None, "b"})->isEmpty);

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num2"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num3"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "a"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "b"})->isEmpty);


  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num1"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num2"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num3"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "a"})->isEmpty);
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "b"})->isEmpty);
}

TEST_CASE("Test self call") {
  string input = "procedure a {"
                 "  call a;"
                 "}";
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  REQUIRE_THROWS_AS(spDriver.parseSource(input, &pkbWriter), SPError);
}

TEST_CASE("Test While Pattern") {
  string input = "procedure printResults {\n"
                 "  while ((((a<b)&&(c>d))&&((c==d)&&(e!=f)))||(((a<b)&&(c>d))&&((c==d)&&(e!=f)))) {"
                 "    print pp;"
                 "  }"
                 "}";
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(input, &pkbWriter);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "a"})->isEmpty);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "b"})->isEmpty);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "c"})->isEmpty);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "d"})->isEmpty);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "e"})->isEmpty);
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "f"})->isEmpty);
}

TEST_CASE("Test acyclic call") {
  string input = "procedure a {"
                 "  call b;"
                 "}"
                 "procedure b {"
                 "  call c;"
                 "}"
                 "procedure c {"
                 "  call a;"
                 "}";

  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  REQUIRE_THROWS_AS(spDriver.parseSource(input, &pkbWriter), SPError);
}

TEST_CASE("Test If Pattern") {
  string input = "procedure printResults {\n"
                 "  if ((((a<b)&&(c>d))&&((c==d)&&(e!=f)))||(((a<b)&&(c>d))&&((c==d)&&(e!=f)))) then {"
                 "    print pp;"
                 "  } else {"
                 "    read pp;"
                 "  }"
                 "}";

  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(input, &pkbWriter);

  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "a"})->isEmpty);
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "b"})->isEmpty);
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "c"})->isEmpty);
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "d"})->isEmpty);
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "e"})->isEmpty);
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "f"})->isEmpty);
}


TEST_CASE("Test Duplicate proc") {
  string input = "procedure a {"
                 "  call b;"
                 "}"
                 "procedure a {"
                 "  call c;"
                 "}";

  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  REQUIRE_THROWS_AS(spDriver.parseSource(input, &pkbWriter), SPError);
}

TEST_CASE("Test Non existent proc") {
  string input = "procedure a {"
                 "  call b;"
                 "}"
                 "procedure b {"
                 "  call c;"
                 "}";

  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  REQUIRE_THROWS_AS(spDriver.parseSource(input, &pkbWriter), SPError);
}
