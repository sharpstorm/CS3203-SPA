#include <vector>

#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/SourceParser.h"
#include "../../../../spa/src/sp/extractor/TreeWalker.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/ParentExtractor.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "catch.hpp"

using std::make_shared, std::make_unique;

vector<pair<int, int>> executeParentExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<Extractor*> extractors;
  auto parentExtractor = make_unique<ParentExtractor>(&stubby);
  extractors.push_back(parentExtractor.get());
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, &extractors);
  return stubby.parentStore;
}

TEST_CASE("ParentExtractor Simple Statement List") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "read num3;"
      "}";

  Util u;
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.isSize(v, 0));
}

TEST_CASE("ParentExtractor Statement with If") {
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
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 3, 5));
  REQUIRE(u.contains(v, 3, 6));
  REQUIRE(u.isSize(v, 3));
}

TEST_CASE("ParentExtractor Simple with While") {
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
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 3, 5));
  REQUIRE(u.isSize(v, 2));
}

TEST_CASE("ParentExtractor If in While loop") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "if (num1 == num2) then {"
      "num2 = num2 + 1;"
      "} else {}"
      "num1 = num1 + 1;"
      "print num1;"
      "}"
      "print num2;"
      "}";

  Util u;
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(u.contains(v, 3, 6));
  REQUIRE(u.contains(v, 3, 7));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("ParentExtractor While in If") {
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
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(u.contains(v, 4, 6));
  REQUIRE(u.contains(v, 3, 7));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("ParentExtractor While in Else") {
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
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 3, 5));
  REQUIRE(u.contains(v, 5, 6));
  REQUIRE(u.contains(v, 5, 7));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("ParentExtractor Triple-While Chain") {
  string input =
      "procedure simple {"
      "read num1;"
      "read num2;"
      "while (num1 < num2) {"
      "while (num1 > 0) {"
      "while (num2 > 0){"
      "num2 = num2 + 1;"
      "}"
      "num1 = num1 + 1;"
      "}"
      "print num1;"
      "}"
      "print num2;"
      "}";

  Util u;
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 3, 8));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(u.contains(v, 4, 7));
  REQUIRE(u.contains(v, 5, 6));
  REQUIRE(u.isSize(v, 5));
}

TEST_CASE("ParentExtractor Triple-If Chain") {
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
  vector<pair<int, int>> v = executeParentExtractor(input);
  REQUIRE(u.contains(v, 3, 4));
  REQUIRE(u.contains(v, 4, 5));
  REQUIRE(u.contains(v, 5, 6));
  REQUIRE(u.contains(v, 5, 7));
  REQUIRE(u.contains(v, 4, 8));
  REQUIRE(u.contains(v, 4, 9));
  REQUIRE(u.contains(v, 3, 10));
  REQUIRE(u.contains(v, 3, 11));
  REQUIRE(u.isSize(v, 8));
}
