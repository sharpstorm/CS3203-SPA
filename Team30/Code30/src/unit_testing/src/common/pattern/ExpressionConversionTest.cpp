#include <memory>

#include "catch.hpp"
#include "common/pattern/PatternConverter.h"
#include "TestASTProvider.h"

using std::make_shared, std::make_unique, std::unique_ptr;

TEST_CASE("ExpressionSequence Conversion - Balanced") {
  PKB pkb;
  pkb.variableStorage->insert("x");
  pkb.variableStorage->insert("y");
  pkb.variableStorage->insert("z");
  unique_ptr<PkbQueryHandler> handler = make_unique<PkbQueryHandler>(&pkb);
  unique_ptr<OverrideTable> table = make_unique<OverrideTable>();
  unique_ptr<QueryCache> cache = make_unique<QueryCache>();
  QueryExecutorAgent agent = QueryExecutorAgent(handler.get(), table.get(), cache.get());

  TestASTProvider treeProvider;
  auto expr = PatternConverter::convertASTToPostfix(
      treeProvider.balancedTree.get(), agent);

  // x * y + x * z;
//  ExpressionSequence expected{ "x", "y", "*", "x", "z", "*", "+" };
  ExpressionSequence expected{ 1, 2, TRIE_TIMES, 1, 3, TRIE_TIMES, TRIE_PLUS };
  REQUIRE(*expr == expected);
}

TEST_CASE("ExpressionSequence Conversion - Right Heavy") {
  PKB pkb;
  pkb.variableStorage->insert("x");
  pkb.variableStorage->insert("y");
  pkb.variableStorage->insert("z");
  unique_ptr<PkbQueryHandler> handler = make_unique<PkbQueryHandler>(&pkb);
  unique_ptr<OverrideTable> table = make_unique<OverrideTable>();
  unique_ptr<QueryCache> cache = make_unique<QueryCache>();
  QueryExecutorAgent agent = QueryExecutorAgent(handler.get(), table.get(), cache.get());

  TestASTProvider treeProvider;
  auto expr = PatternConverter::convertASTToPostfix(
      treeProvider.rightHeavyTree.get(), agent);

  // x + y * z;
  ExpressionSequence expected{ 1, 2, 3, TRIE_TIMES, TRIE_PLUS };
  REQUIRE(*expr == expected);
}

TEST_CASE("ExpressionSequence Conversion - Left Heavy") {
  PKB pkb;
  pkb.variableStorage->insert("x");
  pkb.variableStorage->insert("y");
  pkb.variableStorage->insert("z");
  unique_ptr<PkbQueryHandler> handler = make_unique<PkbQueryHandler>(&pkb);
  unique_ptr<OverrideTable> table = make_unique<OverrideTable>();
  unique_ptr<QueryCache> cache = make_unique<QueryCache>();
  QueryExecutorAgent agent = QueryExecutorAgent(handler.get(), table.get(), cache.get());

  TestASTProvider treeProvider;
  auto expr = PatternConverter::convertASTToPostfix(
      treeProvider.leftHeavyTree.get(), agent);

  // x * y + z;
  ExpressionSequence expected{ 1, 2, TRIE_TIMES, 3, TRIE_PLUS };
  REQUIRE(*expr == expected);
}
