#include <vector>
#include <utility>
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/SourceParser.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"

using std::vector, std::string, std::pair, std::make_unique;

StubPkb executeModifiesExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto modifiesExtractor = make_unique<ModifiesExtractor>(&stubby);
  extractors.push_back(modifiesExtractor.get());
  ASTPtr ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return stubby;
}

TEST_CASE("Modifies Extractor - Read statement") {
  string input = "procedure proc {\n"
                 "read a;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
}

TEST_CASE("Modifies Extractor - Assign statement") {
  string input = "procedure proc {\n"
                 "a = b;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
}

TEST_CASE("Modifies Extractor - Single If statement") {
  string input = "procedure proc {\n"
                 "if (!(a < b)) then {"
                 "  a = b;"
                 "} else {"
                 "  b = a;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.isSize(v, 4));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
  REQUIRE(u.contains(v2, "proc", "b"));
}

TEST_CASE("Modifies Extractor - Single While statement") {
  string input = "procedure proc {\n"
                 "while (a < b) {"
                 "a = b;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 2));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
}

TEST_CASE("Modifies Extractor - Nested If in While") {
  string input = "procedure proc {\n"
                 "  while (a < b) {"
                 "    if (a != b) then {"
                 "      c = 1;"
                 "    } else {"
                 "      read d;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 3, "c"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 2, "c"));
  REQUIRE(u.contains(v, 4, "d"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.isSize(v, 6));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "c"));
  REQUIRE(u.contains(v2, "proc", "d"));
}

TEST_CASE("Modifies Extractor - Nested While in If: then statementList") {
  string input = "procedure proc {\n"
                 "  if (a < 1) then {"
                 "    while (a < 2) {"
                 "      b = 2;"
                 "    }"
                 "  } else { read x; }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.contains(v, 4, "x"));
  REQUIRE(u.contains(v, 1, "x"));
  REQUIRE(u.isSize(v, 5));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "b"));
  REQUIRE(u.contains(v2, "proc", "x"));
}

TEST_CASE("Modifies Extractor - Nested While in If: else statementList") {
  string input = "procedure proc {\n"
                 "  if (a < 1) then { read x; }"
                 "  else {"
                 "    while (a < 3) {"
                 "      b = 1;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 2, "x"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "x"));
  REQUIRE(u.contains(v, 3, "b"));
  REQUIRE(u.contains(v, 4, "b"));
  REQUIRE(u.isSize(v, 5));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "b"));
  REQUIRE(u.contains(v2, "proc", "x"));
}

TEST_CASE("Modifies Extractor - Nested While in While in While") {
  string input = "procedure proc {\n"
                 "  while (a < 3) {"
                 "    while (a < 2) {"
                 "      while (a < 1) {"
                 "        a = 1;"
                 "      }"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 4, "a"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 3, "a"));
  REQUIRE(u.isSize(v, 4));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
}

TEST_CASE("Modifies Extractor - Nested If in If in If") {
  string input = "procedure proc {\n"
                 "  if (a < 3) then {"
                 "    if (a < 2) then {"
                 "      if (a < 1) then {"
                 "        a = 1;"
                 "      } else { x = 1; }"
                 "    } else { print y; }"
                 "  } else { print y; }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 4, "a"));
  REQUIRE(u.contains(v, 5, "x"));
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 2, "a"));
  REQUIRE(u.contains(v, 3, "a"));
  REQUIRE(u.contains(v, 3, "x"));
  REQUIRE(u.contains(v, 2, "x"));
  REQUIRE(u.contains(v, 1, "x"));
  REQUIRE(u.isSize(v, 8));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc", "a"));
  REQUIRE(u.contains(v2, "proc", "x"));
}

TEST_CASE("Modifies Extractor - Multi Proc") {
  string input = "procedure proc1 {\n"
                 "a = 1;"
                 "}"
                 "procedure proc2 {\n"
                 "b = 1;"
                 "}"
                 "procedure proc3 {\n"
                 "c = 1;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeModifiesExtractor(input).modifiesStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.contains(v, 3, "c"));
  REQUIRE(u.isSize(v, 3));

  vector<pair<string, string>> v2 = executeModifiesExtractor(input).modifiesPStore;
  REQUIRE(u.contains(v2, "proc1", "a"));
  REQUIRE(u.contains(v2, "proc2", "b"));
  REQUIRE(u.contains(v2, "proc3", "c"));
}
