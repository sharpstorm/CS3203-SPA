#include <vector>
#include <utility>
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/PatternExtractor.h"
#include "sp/extractor/TreeWalker.h"
#include "sp/SourceParser.h"
#include "../StubWriter.cpp"
#include "../Util.cpp"
#include "common/ASTNode/math/math_operand/PlusASTNode.h"
#include "common/ASTNode/math/math_operand/MinusASTNode.h"
#include "common/ASTNode/math/math_operand/TimesASTNode.h"

using std::vector, std::string, std::pair;

vector<pair<int, shared_ptr<IASTNode>>> executePatternExtractor(string input) {
  TreeWalker treeWalker;
  PKB pkb;
  StubPkb stubby(&pkb);
  SourceParser parser;
  vector<shared_ptr<Extractor>> extractors;
  extractors.push_back(shared_ptr<AbstractExtractor>
                           (new PatternExtractor(&stubby)));
  AST ast = parser.parseSource(input);
  treeWalker.walkAST(ast, extractors);
  return stubby.patternStore;
}

TEST_CASE("Pattern Extractor - Simple Assign") {
  string input = "procedure printResults {\n"
                 "a = b;"
                 "}";
  Util u;
  vector<pair<int, shared_ptr<IASTNode>>> v = executePatternExtractor(input);
  VariableASTNode node("b");
  REQUIRE(node.isEquals(v[0].second.get()));
}

TEST_CASE("Pattern Extractor - Simple Plus") {
  string input = "procedure printResults {\n"
                 "a = b + c;"
                 "}";
  Util u;
  vector<pair<int, shared_ptr<IASTNode>>> v = executePatternExtractor(input);
  PlusASTNode plus;
  VariableASTNode n1("b");
  VariableASTNode n2("c");
  REQUIRE(plus.isEquals(v[0].second.get()));
  REQUIRE(n1.isEquals(v[0].second.get()->getChild(0).get()));
  REQUIRE(n2.isEquals(v[0].second.get()->getChild(1).get()));
}

TEST_CASE("Pattern Extractor - Longer expression") {
  string input = "procedure printResults {\n"
                 "a = b + c * d - e;"
                 "}";
  Util u;
  vector<pair<int, shared_ptr<IASTNode>>> v = executePatternExtractor(input);
  PlusASTNode plus;
  MinusASTNode minus;
  TimesASTNode times;
  VariableASTNode b("b");
  VariableASTNode c("c");
  VariableASTNode d("d");
  VariableASTNode e("e");
  REQUIRE(minus.isEquals(v[0].second.get()));
  REQUIRE(plus.isEquals(v[0].second.get()->getChild(0).get()));
  REQUIRE(b.isEquals(v[0].second.get()->getChild(0).get()->getChild(0).get()));
  REQUIRE(times.isEquals(v[0].second.get()->getChild(0).get()->getChild(1).get()));
  REQUIRE(c.isEquals(v[0].second.get()->getChild(0).get()
                         ->getChild(1).get()->getChild(0).get()));
  REQUIRE(d.isEquals(v[0].second.get()->getChild(0).get()
                         ->getChild(1).get()->getChild(1).get()));
  REQUIRE(e.isEquals(v[0].second.get()->getChild(1).get()));
}