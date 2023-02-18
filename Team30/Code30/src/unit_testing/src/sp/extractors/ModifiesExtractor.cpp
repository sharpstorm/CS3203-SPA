#include <vector>
#include <utility>
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/SourceParser.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"

using std::vector, std::string, std::pair;

vector<pair<int, string>> executeModifiesExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<shared_ptr<Extractor>> extractors;
  extractors.push_back(shared_ptr<AbstractExtractor>
                           (new ModifiesExtractor(&stubby)));
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, extractors);
  return stubby.modifiesStore;
}

TEST_CASE("Modifies Extractor - Read statement") {
  string input = "procedure printResults {\n"
                 "read a;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));
}

TEST_CASE("Modifies Extractor - Assign statement") {
  string input = "procedure printResults {\n"
                 "a = b;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));
}

TEST_CASE("Modifies Extractor - Single If statement") {
  string input = "procedure printResults {\n"
                 "if (!(a < b)) then {"
                 "  a = b;"
                 "} else {"
                 "  b = a;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("Modifies Extractor - Single While statement") {
  string input = "procedure printResults {\n"
                 "while (a < b) {"
                 "a = b;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 2));
}

TEST_CASE("Modifies Extractor - Nested If in While") {
  string input = "procedure printResults {\n"
                 "  while (a < b) {"
                 "    if (a != b) then {"
                 "      c = 1;"
                 "    } else {"
                 "      read d;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 3, "c"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 2, "c"));
  REQUIRE(u.contains(v, 4, "d"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.isSize(v, 6));
}

TEST_CASE("Modifies Extractor - Nested While in If: then statementList") {
  string input = "procedure printResults {\n"
                 "  if (a < 1) then {"
                 "    while (a < 2) {"
                 "      b = 2;"
                 "    }"
                 "  } else {}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.isSize(v, 3));
}

TEST_CASE("Modifies Extractor - Nested While in If: else statementList") {
  string input = "procedure printResults {\n"
                 "  if (a < 1) then {}"
                 "  else {"
                 "    while (a < 3) {"
                 "      b = 1;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.isSize(v, 3));
}

TEST_CASE("Modifies Extractor - Nested While in While in While") {
  string input = "procedure printResults {\n"
                 "  while (a < 3) {"
                 "    while (a < 2) {"
                 "      while (a < 1) {"
                 "        a = 1;"
                 "      }"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 4, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 3, "a"));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("Modifies Extractor - Nested If in If in If") {
  string input = "procedure printResults {\n"
                 "  if (a < 3) then {"
                 "    if (a < 2) then {"
                 "      if (a < 1) then {"
                 "        a = 1;"
                 "      } else {}"
                 "    } else {}"
                 "  } else {}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input);
  REQUIRE(u.contains(v, 4, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 3, "a"));
  REQUIRE(u.isSize(v, 4));
}
