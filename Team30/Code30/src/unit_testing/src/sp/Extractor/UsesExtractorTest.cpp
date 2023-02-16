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
}

TEST_CASE("Uses Extractor - Assign statement 1") {
  string input = "procedure printResults {\n"
                 "x = a + b + c + d + e + f + g + h;"
                 "}";
  Util u;
  try {
    vector<pair<int, string>> v = executeUsesExtractor(input);
    REQUIRE(u.contains(v, 1, "a"));
    REQUIRE(u.contains(v, 1, "b"));
    REQUIRE(u.contains(v, 1, "c"));
    REQUIRE(u.contains(v, 1, "d"));
    REQUIRE(u.contains(v, 1, "e"));
    REQUIRE(u.contains(v, 1, "f"));
    REQUIRE(u.contains(v, 1, "g"));
    REQUIRE(u.contains(v, 1, "h"));
  } catch (SPError s) {
    std::cout << s.message << std::endl;
  }
}

TEST_CASE("Uses Extractor - Assign statement 2") {
  string input = "procedure printResults {\n"
                 "x = a * b * c;"
                 "}";
  Util u;
  try {
    vector<pair<int, string>> v = executeUsesExtractor(input);
    REQUIRE(u.contains(v, 1, "a"));
    REQUIRE(u.contains(v, 1, "b"));
    REQUIRE(u.contains(v, 1, "c"));
  } catch (SPError s) {
    std::cout << s.message << std::endl;
  }
}