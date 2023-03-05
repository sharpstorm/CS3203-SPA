#include "catch.hpp"
#include "TestASTProvider.h"
#include "common/pattern/PatternConverter.h"
#include "sp/SourceParser.h"

void assertMatchFull(PatternTrie* trie, ExpressionSequence expected) {
  REQUIRE(trie->isMatchPartial(&expected));
  REQUIRE(trie->isMatchFull(&expected));
}

void assertMatchPartial(PatternTrie* trie, ExpressionSequence expected) {
  REQUIRE(trie->isMatchPartial(&expected));
  REQUIRE(!trie->isMatchFull(&expected));
}

void assertNotMatch(PatternTrie* trie, ExpressionSequence expected) {
  REQUIRE(!trie->isMatchPartial(&expected));
  REQUIRE(!trie->isMatchFull(&expected));
}

TEST_CASE("Trie Building - Simple Tree") {
  TestASTProvider treeProvider;
  auto trie = PatternConverter::convertASTToTrie(
      treeProvider.leftHeavyTree->getRoot());
  auto triePtr = trie.get();

  // x * y + z;
  assertMatchFull(triePtr, {"x", "y", "*", "z", "+"});

  assertMatchPartial(triePtr, {"x", "y", "*"});
  assertMatchPartial(triePtr, {"x"});
  assertMatchPartial(triePtr, {"y"});
  assertMatchPartial(triePtr, {"z"});

  assertNotMatch(triePtr, {"x", "z", "+"});
  assertNotMatch(triePtr, {"y", "z", "+"});
  assertNotMatch(triePtr, {"w", "z", "+"});
}

TEST_CASE("Trie Building - Heavier Tree") {
  TestASTProvider treeProvider;
  auto trie = PatternConverter::convertASTToTrie(
      treeProvider.balancedTree->getRoot());
  auto triePtr = trie.get();

  // x * y + x * z;
  assertMatchFull(triePtr, {"x", "y", "*", "x", "z", "*", "+"});

  assertMatchPartial(triePtr, {"x", "y", "*"});
  assertMatchPartial(triePtr, {"x", "z", "*"});
  assertMatchPartial(triePtr, {"x"});
  assertMatchPartial(triePtr, {"y"});
  assertMatchPartial(triePtr, {"z"});

  assertNotMatch(triePtr, {"y", "x", "+"});
  assertNotMatch(triePtr, {"x", "y", "*", "x", "+"});
  assertNotMatch(triePtr, {"x", "y", "*", "z", "+"});
  assertNotMatch(triePtr, {"y", "x", "z", "*", "+"});
  assertNotMatch(triePtr, {"x", "x", "z", "*", "+"});
}

TEST_CASE("Trie Building - Stress Tree") {
  auto exprTree = SourceParser().parseExpression(
      "call + procedure + assign - if + 20000 - read + call - read - if * print / while * 1000 + while + call + procedure + assign + if - while / 299385 - asdfasdf123123");
  auto trie = PatternConverter::convertASTToTrie(exprTree.getRoot());
}

