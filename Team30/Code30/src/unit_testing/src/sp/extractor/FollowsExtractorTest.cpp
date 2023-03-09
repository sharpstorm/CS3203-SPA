#include "sp/ast/AST.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

using std::vector, std::string, std::pair, std::make_unique;

vector<pair<int, int>> executeFollowsExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto followsExtractor = make_unique<FollowsExtractor>(&stubby);
  extractors.push_back(followsExtractor.get());
  ASTPtr ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return stubby.followsStore;
}

TEST_CASE("FollowsExtractor Simple Statement list") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.isSize(v, 2));
}

TEST_CASE("FollowsExtractor Statement with If") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(!u.contains(v, 3, 4));  // Statement 3 does NOT follow 4
  REQUIRE(!u.contains(v, 3, 6));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(!u.contains(v, 5, 6));
  REQUIRE(u.isSize(v, 3));
}

TEST_CASE("FollowsExtractor Statement with While loop") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.contains(v, 3, 6));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(!u.contains(v, 3, 4));  // Statement 3 does NOT follow 4
  REQUIRE(!u.contains(v, 5, 6));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("FollowsExtractor If in While loop") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(!u.contains(v, 3, 4));
  REQUIRE(!u.contains(v, 4, 5));
  REQUIRE(!u.contains(v, 5, 6));
  REQUIRE(u.contains(v, 3, 9));
  REQUIRE(u.isSize(v, 5));
}

TEST_CASE("FollowsExtractor While in If") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.contains(v, 3, 8));
  REQUIRE(u.contains(v, 5, 6));
  REQUIRE(!u.contains(v, 3, 4));
  REQUIRE(!u.contains(v, 4, 5));
  REQUIRE(!u.contains(v, 7, 8));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("FollowsExtractor While in Else") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.contains(v, 3, 8));
  REQUIRE(u.contains(v, 6, 7));
  REQUIRE(!u.contains(v, 5, 6));
  REQUIRE(!u.contains(v, 3, 4));
  REQUIRE(!u.contains(v, 4, 5));
  REQUIRE(!u.contains(v, 7, 8));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("FollowsExtractor Triple-While Chain") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.contains(v, 3, 9));
  REQUIRE(u.contains(v, 4, 8));
  REQUIRE(u.contains(v, 5, 7));
  REQUIRE(u.isSize(v, 5));
}

TEST_CASE("FollowsExtracotr Triple-If Chain") {
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

  Util u;
  vector<pair<int, int>> v = executeFollowsExtractor(input);
  REQUIRE(u.contains(v, 1, 2));
  REQUIRE(u.contains(v, 2, 3));
  REQUIRE(u.contains(v, 3, 12));
  REQUIRE(u.contains(v, 4, 10));
  REQUIRE(u.contains(v, 5, 8));
  REQUIRE(u.isSize(v, 5));
}
