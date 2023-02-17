#include <vector>
#include <utility>
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/ModifiesExtractor.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/SourceParser.h"
#include "../StubWriter.cpp"
#include "sp/extractor/concrete_extractors/UsesExtractor.h"
#include "../Util.cpp"
#include "sp/errors/SPError.h"
#include <iostream>

using std::vector, std::string, std::pair;


vector<pair<int, string>> executeUsesExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<shared_ptr<Extractor>> extractors;
  extractors.push_back(shared_ptr<AbstractExtractor>
                           (new UsesExtractor(&stubby)));
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, extractors);
  return stubby.usesStore;
}

TEST_CASE("Uses Extractor - Print statement") {
  string input = "procedure printResults {\n"
                 "print a;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));
}

TEST_CASE("Uses Extractor - Assign statement 1") {
  string input = "procedure printResults {\n"
                 "x = a;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.isSize(v, 1));
}

TEST_CASE("Uses Extractor - Assign statement 2") {
  string input = "procedure printResults {\n"
                 "x = (a * b / c % d) + e - f ;"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "e"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(u.isSize(v, 6));
}

TEST_CASE("Uses Extractor - Single If statement") {
  string input = "procedure printResults {\n"
                 "if (!(a < b)) then {"
                 "  c = d;"
                 "} else {"
                 "  e = f;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.contains(v, 3, "f"));
  REQUIRE(u.isSize(v, 6));
}

TEST_CASE("Uses Extractor - Single While statement") {
  string input = "procedure printResults {\n"
                 "while (a < b) {"
                 "c = d;"
                 "}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.isSize(v, 4));
}

TEST_CASE("Uses Extractor - Nested If in While") {
  string input = "procedure printResults {\n"
                 "  while (a < b) {"
                 "    if (c != d) then {"
                 "      e = f;"
                 "    } else {"
                 "      print g;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(u.contains(v, 1, "g"));
  REQUIRE(u.contains(v, 2, "c"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.contains(v, 2, "f"));
  REQUIRE(u.contains(v, 2, "g"));
  REQUIRE(u.contains(v, 3, "f"));
  REQUIRE(u.contains(v, 4, "g"));
  REQUIRE(u.isSize(v, 12));
}

TEST_CASE("Uses Extractor - Nested While in If: then statementList") {
  string input = "procedure printResults {\n"
                 "  if (a < 1) then {"
                 "    while (b < 2) {"
                 "      c = d;"
                 "    }"
                 "  } else {}"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.contains(v, 3, "d"));
  REQUIRE(u.isSize(v, 6));
}

TEST_CASE("Uses Extractor - Nested While in If: else statementList") {
  string input = "procedure printResults {\n"
                 "  if (a < 1) then {}"
                 "  else {"
                 "    while (b < 3) {"
                 "      c = d;"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 2, "b"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.contains(v, 3, "d"));
  REQUIRE(u.isSize(v, 6));
}

TEST_CASE("Uses Extractor - Nested While in While in While") {
  string input = "procedure printResults {\n"
                 "  while (a < b) {"
                 "    while (c < d) {"
                 "      while (e < f) {"
                 "        g = h;"
                 "      }"
                 "    }"
                 "  }"
                 "}";
  Util u;
  vector<pair<int, string>> v = executeUsesExtractor(input);
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "e"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(u.contains(v, 1, "h"));
  REQUIRE(u.contains(v, 2, "c"));
  REQUIRE(u.contains(v, 2, "d"));
  REQUIRE(u.contains(v, 2, "e"));
  REQUIRE(u.contains(v, 2, "f"));
  REQUIRE(u.contains(v, 2, "h"));
  REQUIRE(u.contains(v, 3, "e"));
  REQUIRE(u.contains(v, 3, "f"));
  REQUIRE(u.contains(v, 3, "h"));
  REQUIRE(u.contains(v, 4, "h"));
  REQUIRE(u.isSize(v, 16));
}