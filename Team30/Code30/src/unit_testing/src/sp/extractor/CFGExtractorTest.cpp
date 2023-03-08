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

vector<CFGPtr> executeCFGExtractor(string input) {
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

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
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

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3}, {-1, 4, 6}, {5}, {-1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
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

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3}, {6, 4}, {5}, {3, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
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

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2},  {3}, {9, 4},  {7, 5, 6}, {-1},
                            {-1}, {8}, {3, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
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

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3},     {8, 4, 7}, {-1, 5},
                            {6}, {4, -1}, {-1},      {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor While in Else") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "num2 = num2 + 1;"
      "} else {"
      "while (num1 < num2) {"
      "num1 = num1 + 1;"
      "print num1;"
      "}}"
      "print num2;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2},     {3}, {8, 4, 5}, {-1},
                            {-1, 6}, {7}, {5, -1},   {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor Triple-While Chain") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "  while (num1 > 0) {"
      "    while (num2 > 0) {"
      "      num2 = num2 + 1;"
      "    }"
      "    num1 = num1 + 1;"
      "  }"
      "  print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2},     {3},     {9, 4},  {8, 5}, {7, 6},
                            {5, -1}, {4, -1}, {3, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFG Triple-If Chain") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "  if (num1 > 0) then {"
      "    if (num2 > 0) then {"
      "      num2 = num2 + 1;"
      "    } else {"
      "      print num1;"
      "  } num1 = num1 + 1;"
      "  } else {"
      "    print num2;"
      "} print num1;"
      "} else {"
      "print num1;"
      "}"
      "print num2;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2},       {3},  {12, 4, 11}, {10, 5, 9},
                            {8, 6, 7}, {-1}, {-1},        {-1},
                            {-1},      {-1}, {-1},        {-1}};

  REQUIRE(setofCFGs.size() == 1);
  REQUIRE(setofCFGs[0]->getLinks() == link);
}

TEST_CASE("CFGExtractor Two Procedures with Simple Statement list") {
  string input =
      "procedure simple {"
      "read num0;"
      "read num1;"
      "read num2;"
      "read num3;"
      "}"
      "procedure simpleTwo {"
      "read num4;"
      "read num5;"
      "read num6;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3}, {4}, {-1}};
  vector<list<int>> linkTwo = {{6}, {7}, {-1}};

  REQUIRE(setofCFGs.size() == 2);
  REQUIRE(setofCFGs[0]->getName() == "simple");
  REQUIRE(setofCFGs[0]->getLinks() == link);

  REQUIRE(setofCFGs[1]->getName() == "simpleTwo");
  REQUIRE(setofCFGs[1]->getLinks() == linkTwo);
}

TEST_CASE("CFGExtractor Three Procedures with Simple Statement list") {
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
      "}"
      "procedure simpleThree {"
      "read num7;"
      "read num8;"
      "read num9;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2}, {3}, {-1}};
  vector<list<int>> linkTwo = {{5}, {6}, {-1}};
  vector<list<int>> linkThree = {{8}, {9}, {-1}};

  REQUIRE(setofCFGs.size() == 3);
  REQUIRE(setofCFGs[0]->getName() == "simple");
  REQUIRE(setofCFGs[0]->getLinks() == link);

  REQUIRE(setofCFGs[1]->getName() == "simpleTwo");
  REQUIRE(setofCFGs[1]->getLinks() == linkTwo);

  REQUIRE(setofCFGs[2]->getName() == "simpleThree");
  REQUIRE(setofCFGs[2]->getLinks() == linkThree);
}

TEST_CASE("CFGExtractor Two Procedures with complex statements") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "if (num1 < num2) then {"
      "  if (num1 > 0) then {"
      "    if (num2 > 0) then {"
      "      num2 = num2 + 1;"
      "    } else {"
      "      print num1;"
      "  } num1 = num1 + 1;"
      "  } else {"
      "    print num2;"
      "} print num1;"
      "} else {"
      "print num1;"
      "}"
      "print num2;"
      "}"
      "procedure simpleTwo {"
      "read num3;"
      "read num4;"
      "while (num3 < num4) {"
      "  while (num3 > 0) {"
      "    while (num4 > 0) {"
      "      num4 = num4 + 1;"
      "    }"
      "    num = num4 + 1;"
      "  }"
      "  print num4;"
      "}"
      "print num4;"
      "}";

  vector<CFGPtr> setofCFGs = executeCFGExtractor(input);
  vector<list<int>> link = {{2},       {3},  {12, 4, 11}, {10, 5, 9},
                            {8, 6, 7}, {-1}, {-1},        {-1},
                            {-1},      {-1}, {-1},        {-1}};

  vector<list<int>> linkTwo = {{14},     {15},     {21, 16}, {20, 17}, {19, 18},
                               {17, -1}, {16, -1}, {15, -1}, {-1}};

  REQUIRE(setofCFGs.size() == 2);
  REQUIRE(setofCFGs[0]->getName() == "simple");
  REQUIRE(setofCFGs[0]->getLinks() == link);

  REQUIRE(setofCFGs[1]->getName() == "simpleTwo");
  REQUIRE(setofCFGs[1]->getLinks() == linkTwo);
}
