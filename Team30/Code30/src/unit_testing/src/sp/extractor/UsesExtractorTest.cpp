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

using std::vector, std::string, std::pair, std::make_unique;

StubPkb executeUsesExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto usesExtractor = make_unique<UsesExtractor>(&stubby);
  extractors.push_back(usesExtractor.get());
  ASTPtr ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return stubby;
}

TEST_CASE("Uses Extractor - Print statement") {
  string input = "procedure proc {\n"
                 "print a;"
                 "}";

  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.getUsesSize() == 1);
  REQUIRE(v.getUsesPSize() == 1);
}

TEST_CASE("Uses Extractor - Assign statement 1") {
  string input = "procedure proc {\n"
                 "x = a;"
                 "}";

  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.getUsesSize() == 1);
  REQUIRE(v.getUsesPSize() == 1);
}

TEST_CASE("Uses Extractor - Assign statement 2") {
  string input = "procedure proc {\n"
                 "x = (a * b / c % d) + e - f ;"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "c"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(1, "proc", "e"));
  REQUIRE(v.hasUses(1, "proc", "f"));
  REQUIRE(v.getUsesSize() == 6);
  REQUIRE(v.getUsesPSize() == 6);
}

TEST_CASE("Uses Extractor - Single If statement") {
  string input = "procedure proc {\n"
                 "if (!(a < b)) then {"
                 "  c = d;"
                 "} else {"
                 "  e = f;"
                 "}"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(1, "proc", "f"));
  REQUIRE(v.hasUses(2, "d"));
  REQUIRE(v.hasUses(3, "f"));

  REQUIRE(v.getUsesSize() == 6);
  REQUIRE(v.getUsesPSize() == 4);
}

TEST_CASE("Uses Extractor - Single While statement") {
  string input = "procedure proc {\n"
                 "while (a < b) {"
                 "c = d;"
                 "}"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(2, "d"));

  REQUIRE(v.getUsesSize() == 4);
  REQUIRE(v.getUsesPSize() == 3);
}

TEST_CASE("Uses Extractor - Nested If in While") {
  string input = "procedure proc {\n"
                 "  while (a < b) {"
                 "    if (c != d) then {"
                 "      e = f;"
                 "    } else {"
                 "      print g;"
                 "    }"
                 "  }"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "c"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(1, "proc", "f"));
  REQUIRE(v.hasUses(1, "proc", "g"));

  REQUIRE(v.hasUses(2, "c"));
  REQUIRE(v.hasUses(2, "d"));
  REQUIRE(v.hasUses(2, "f"));
  REQUIRE(v.hasUses(2, "g"));
  REQUIRE(v.hasUses(3, "f"));
  REQUIRE(v.hasUses(4, "g"));

  REQUIRE(v.getUsesSize() == 12);
  REQUIRE(v.getUsesPSize() == 6);
}

TEST_CASE("Uses Extractor - Nested While in If: then statementList") {
  string input = "procedure proc {\n"
                 "  if (a < 1) then {"
                 "    while (b < 2) {"
                 "      c = d;"
                 "    }"
                 "  } else { print x; }"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(1, "proc", "x"));

  REQUIRE(v.hasUses(2, "b"));
  REQUIRE(v.hasUses(2, "d"));
  REQUIRE(v.hasUses(3, "d"));
  REQUIRE(v.hasUses(4, "x"));

  REQUIRE(v.getUsesSize() == 8);
  REQUIRE(v.getUsesPSize() == 4);
}

TEST_CASE("Uses Extractor - Nested While in If: else statementList") {
  string input = "procedure proc {\n"
                 "  if (a < 1) then { print x; }"
                 "  else {"
                 "    while (b < 3) {"
                 "      c = d;"
                 "    }"
                 "  }"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "x"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "d"));

  REQUIRE(v.hasUses(2, "x"));
  REQUIRE(v.hasUses(3, "b"));
  REQUIRE(v.hasUses(3, "d"));
  REQUIRE(v.hasUses(4, "d"));

  REQUIRE(v.getUsesSize() == 8);
  REQUIRE(v.getUsesPSize() == 4);
}

TEST_CASE("Uses Extractor - Nested While in While in While") {
  string input = "procedure proc {\n"
                 "  while (a < b) {"
                 "    while (c < d) {"
                 "      while (e < f) {"
                 "        g = h;"
                 "      }"
                 "    }"
                 "  }"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc", "a"));
  REQUIRE(v.hasUses(1, "proc", "b"));
  REQUIRE(v.hasUses(1, "proc", "c"));
  REQUIRE(v.hasUses(1, "proc", "d"));
  REQUIRE(v.hasUses(1, "proc", "e"));
  REQUIRE(v.hasUses(1, "proc", "f"));
  REQUIRE(v.hasUses(1, "proc", "h"));

  REQUIRE(v.hasUses(2, "c"));
  REQUIRE(v.hasUses(2, "d"));
  REQUIRE(v.hasUses(2, "e"));
  REQUIRE(v.hasUses(2, "f"));
  REQUIRE(v.hasUses(2, "h"));

  REQUIRE(v.hasUses(3, "e"));
  REQUIRE(v.hasUses(3, "f"));
  REQUIRE(v.hasUses(3, "h"));

  REQUIRE(v.hasUses(4, "h"));

  REQUIRE(v.getUsesSize() == 16);
  REQUIRE(v.getUsesPSize() == 7);
}

TEST_CASE("Uses Extractor - Multi Proc") {
  string input = "procedure proc1 {\n"
                 "a = d;"
                 "}"
                 "procedure proc2 {\n"
                 "b = e;"
                 "}"
                 "procedure proc3 {\n"
                 "c = f;"
                 "}";
  auto v = executeUsesExtractor(input);
  REQUIRE(v.hasUses(1, "proc1", "d"));
  REQUIRE(v.hasUses(2, "proc2", "e"));
  REQUIRE(v.hasUses(3, "proc3", "f"));

  REQUIRE(v.getUsesSize() == 3);
  REQUIRE(v.getUsesPSize() == 3);
}