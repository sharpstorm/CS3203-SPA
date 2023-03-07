#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/ast/CFG.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/CFGExtractor.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"
#include "sp/ast/AST.h"

using std::vector, std::string, std::list, std::make_unique;

vector<CFG*> executeCFGExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<Extractor*> extractors;
  auto cfgExtractor = make_unique<CFGExtractor>(&stubby);
  extractors.push_back(cfgExtractor.get());
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, &extractors);
  return cfgExtractor->getSetOfCFGs();
}

TEST_CASE("CFGExtractor Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3};
  vector<list<int>> link = {{2}, {3}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor Statement with If") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "num1 = num1 + 1;"
      "print num1;"
      "} else {"
      "print num2;"
      "}"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3, 4, 5, 6};
  vector<list<int>> link = {{2}, {3}, {-1, 4, 6}, {5}, {-1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor Statement with While loop") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3, 4, 5, 6};
  vector<list<int>> link = {{2}, {3}, {6, 4}, {5}, {3, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor If in While loop") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "if (num1 == num2) then {"
      "num2 = num2 + 1;"
      "} else {"
      "num2 = 2;"
      "}"
      "num1 = num1 + 1;"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  vector<list<int>> link = {{2},  {3}, {9, 4},  {7, 5, 6}, {-1},
                            {-1}, {8}, {3, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor While in If") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}} else {"
      "num2 = num2 + 1;"
      "}"
      "print num2;"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3, 4, 5, 6, 7, 8};
  vector<list<int>> link = {{2}, {3},     {8, 4, 7}, {-1, 5},
                            {6}, {4, -1}, {-1},      {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor Two Procedures with Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure simpleTwo {"
      "read num4;"
      "read num5;"
      "read num6;"
      "}";

  vector<CFG*> setofCFGs = executeCFGExtractor(input);
  vector<int> nodeMap = {1, 2, 3};
  vector<list<int>> link = {{2}, {3}, {-1}};

  vector<int> nodeMapTwo = {1, 2, 3};
  vector<list<int>> linkTwo = {{2}, {3}, {-1}};

  REQUIRE(setofCFGs.size() == 2);
  REQUIRE(setofCFGs[0]->getName() == "simple");
  REQUIRE(setofCFGs[0]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[0]->getLinks() == link);

  REQUIRE(setofCFGs[1]->getName() == "simpleTwo");
  REQUIRE(setofCFGs[1]->getNodeMap() == nodeMap);
  REQUIRE(setofCFGs[1]->getLinks() == link);
}
