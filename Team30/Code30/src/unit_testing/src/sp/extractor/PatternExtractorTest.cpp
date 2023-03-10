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

StubPkb executePatternExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<IExtractor*> extractors;
  auto patternExtractor = make_unique<PatternExtractor>(&stubby);
  extractors.push_back(patternExtractor.get());
  auto ast = parser.parseSource(input);
  treeWalker.walkAST(ast.get(), &extractors);
  return stubby;
}

TEST_CASE("Pattern Extractor - Simple Assign") {
  string input = "procedure printResults {\n"
                 "a = b;"
                 "}";
  auto v = executePatternExtractor(input).patternStore;
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchFull(&expected));
}

TEST_CASE("Pattern Extractor - Simple Plus") {
  string input = "procedure printResults {\n"
                 "a = b + c;"
                 "}";
  auto v = executePatternExtractor(input).patternStore;
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"c"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
}

TEST_CASE("Pattern Extractor - Longer expression") {
  string input = "procedure printResults {\n"
                 "a = b + c * d - e;"
                 "}";
  auto v = executePatternExtractor(input).patternStore;
  ExpressionSequence expected{"b"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"c"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"d"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
  expected = {"e"};
  REQUIRE(v[0].second->isMatchPartial(&expected));
}

TEST_CASE("Pattern Extractor - While Pattern") {
  string input = "procedure printResults {\n"
                 "  while ((a<b)&&(c>d)) {"
                 "    print pp;"
                 "  }"
                 "}";
  Util u;
  auto v = executePatternExtractor(input).whilePatternStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(v.size() == 4);
}

TEST_CASE("Pattern Extractor - While Pattern Complex") {
  string input = "procedure printResults {\n"
                 "  while ((((a<b)&&(c>d))&&((c==d)&&(e!=f)))||(((a<b)&&(c>d))&&((c==d)&&(e!=f)))) {"
                 "    print pp;"
                 "  }"
                 "}";
  Util u;
  auto v = executePatternExtractor(input).whilePatternStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "e"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(v.size() == 6);
}

TEST_CASE("Pattern Extractor - If Pattern") {
  string input = "procedure printResults {\n"
                 "  if ((a<b)&&(c>d)) then {"
                 "    print pp;"
                 "  } else {"
                 "    read pp;"
                 "  }"
                 "}";
  Util u;
  auto v = executePatternExtractor(input).ifPatternStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(v.size() == 4);
}

TEST_CASE("Pattern Extractor - If Pattern Complex") {
  string input = "procedure printResults {\n"
                 "  if ((((a<b)&&(c>d))&&((c==d)&&(e!=f)))||(((a<b)&&(c>d))&&((c==d)&&(e!=f)))) then {"
                 "    print pp;"
                 "  } else {"
                 "    read pp;"
                 "  }"
                 "}";
  Util u;
  auto v = executePatternExtractor(input).ifPatternStore;
  REQUIRE(u.contains(v, 1, "a"));
  REQUIRE(u.contains(v, 1, "b"));
  REQUIRE(u.contains(v, 1, "c"));
  REQUIRE(u.contains(v, 1, "d"));
  REQUIRE(u.contains(v, 1, "e"));
  REQUIRE(u.contains(v, 1, "f"));
  REQUIRE(v.size() == 6);
}




