#include "catch.hpp"
#include <string>
#include <memory>

#include <unordered_set>
#include "pkb/writers/PkbWriter.h"
#include "sp/errors/SPError.h"
#include "sp/SpDriver.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

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
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 1}, StmtRef{StmtType::None, 2})->empty());
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 2}, StmtRef{StmtType::None, 3})->empty());
  REQUIRE(!queryHandler.queryFollows(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 5})->empty());
}

TEST_CASE("Test Write Parent") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryParent(StmtRef{StmtType::None, 3}, StmtRef{StmtType::None, 4})->empty());
}

TEST_CASE("Test Write Uses") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num1"})->empty());
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num2"})->empty());
  REQUIRE(!queryHandler.queryUses(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "num3"})->empty());
}

TEST_CASE("Test Write Modifies") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 2}, EntityRef{EntityType::Variable, "sum"})->empty());
  REQUIRE(!queryHandler.queryModifies(StmtRef{StmtType::None, 4}, EntityRef{EntityType::Variable, "abc"})->empty());
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
                                   EntityRef{EntityType::Procedure, "average"})->empty());
  REQUIRE(!queryHandler.queryCalls(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"})->empty());
}

TEST_CASE("Test Write Calls star") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "a2"},
                                   EntityRef{EntityType::Procedure, "average"})->empty());
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "a2"})->empty());
  REQUIRE(!queryHandler.queryCallsStar(EntityRef{EntityType::Procedure, "main"},
                                   EntityRef{EntityType::Procedure, "average"})->empty());
}

TEST_CASE("Test Writer Pattern") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource, &pkbWriter);
  PatternTrie* trie = *queryHandler.
          queryAssigns(StmtRef{StmtType::None, 2})
      ->getRightVals().begin();
  ExpressionSequence expected{1};
  REQUIRE(trie->isMatchPartial(&expected));
  expected = {3};
  REQUIRE(trie->isMatchPartial(&expected));
  expected = {4};
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
                                      EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "sum"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "abc"})->empty());

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "flag"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "sum"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "a2"},
                                      EntityRef{EntityType::None, "abc"})->empty());

  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "flag"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "sum"})->empty());
  REQUIRE(!queryHandler.queryModifies(EntityRef{EntityType::None, "main"},
                                      EntityRef{EntityType::None, "abc"})->empty());
}

TEST_CASE("Test UsesP") {
  SpDriver spDriver;
  PKB pkb;
  PkbWriter pkbWriter(&pkb);
  PkbQueryHandler queryHandler(&pkb);
  spDriver.parseSource(testSource2, &pkbWriter);

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num2"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                      EntityRef{EntityType::None, "num3"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                  EntityRef{EntityType::None, "a"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "average"},
                                  EntityRef{EntityType::None, "b"})->empty());

  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num2"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "num3"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "a"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "a2"},
                                  EntityRef{EntityType::None, "b"})->empty());


  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num1"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num2"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "num3"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "a"})->empty());
  REQUIRE(!queryHandler.queryUses(EntityRef{EntityType::None, "main"},
                                  EntityRef{EntityType::None, "b"})->empty());
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
                                          EntityRef{EntityType::None, "a"})->empty());
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "b"})->empty());
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "c"})->empty());
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "d"})->empty());
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "e"})->empty());
  REQUIRE(!queryHandler.queryWhilePattern(StmtRef{StmtType::While, 1},
                                          EntityRef{EntityType::None, "f"})->empty());
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
                                       EntityRef{EntityType::None, "a"})->empty());
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "b"})->empty());
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "c"})->empty());
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "d"})->empty());
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "e"})->empty());
  REQUIRE(!queryHandler.queryIfPattern(StmtRef{StmtType::If, 1},
                                       EntityRef{EntityType::None, "f"})->empty());
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
