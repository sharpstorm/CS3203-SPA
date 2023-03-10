#include <vector>
#include <utility>
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/PatternExtractor.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/SourceParser.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/expression_operand/MinusASTNode.h"
#include "sp/ast/expression_operand/TimesASTNode.h"

using std::vector, std::string, std::pair, std::make_unique;

vector<pair<int, PatternTrieSPtr>> executePatternExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto patternExtractor = make_unique<PatternExtractor>(&stubby);
  extractors.push_back(patternExtractor.get());
  ASTPtr ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return stubby.patternStore;
}

TEST_CASE("Pattern Extractor - Simple Assign") {
  string input = "procedure printResults {\n"
                 "a = b;"
                 "}";
  auto v = executePatternExtractor(input);
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchFull(&expected));
}

TEST_CASE("Pattern Extractor - Simple Plus") {
  string input = "procedure printResults {\n"
                 "a = b + c;"
                 "}";
  auto v = executePatternExtractor(input);
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"c"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
}

TEST_CASE("Pattern Extractor - Longer expression") {
  string input = "procedure printResults {\n"
                 "a = b + c * d - e;"
                 "}";
  auto v = executePatternExtractor(input);
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"c"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"d"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"e"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
}