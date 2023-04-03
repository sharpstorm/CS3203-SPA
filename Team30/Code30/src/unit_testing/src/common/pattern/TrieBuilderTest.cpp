#include "catch.hpp"

#include <memory>

#include "TestASTProvider.h"
#include "common/pattern/PatternConverter.h"
#include "sp/SourceParser.h"

using std::make_unique;

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
  PKB pkb;
  unique_ptr<PkbWriter> writer = make_unique<PkbWriter>(&pkb);
  TestASTProvider treeProvider;
  auto trie = PatternConverter::convertASTToTrie(
      treeProvider.leftHeavyTree->getRoot(), writer.get());
  auto triePtr = trie.get();

  // x * y + z;
  assertMatchFull(triePtr, {1, 2, TRIE_TIMES, 3, TRIE_PLUS});

  assertMatchPartial(triePtr, {1, 2, TRIE_TIMES});
  assertMatchPartial(triePtr, {1});
  assertMatchPartial(triePtr, {2});
  assertMatchPartial(triePtr, {3});

  assertNotMatch(triePtr, {1, 2, TRIE_PLUS});
  assertNotMatch(triePtr, {2, 3, TRIE_PLUS});
  assertNotMatch(triePtr, {4, 3, TRIE_PLUS});
}

TEST_CASE("Trie Building - Heavier Tree") {
  PKB pkb;
  unique_ptr<PkbWriter> writer = make_unique<PkbWriter>(&pkb);
  TestASTProvider treeProvider;
  auto trie = PatternConverter::convertASTToTrie(
      treeProvider.balancedTree->getRoot(), writer.get());
  auto triePtr = trie.get();

  // x * y + x * z;
  assertMatchFull(triePtr, {1, 2, TRIE_TIMES, 1, 3, TRIE_TIMES, TRIE_PLUS});
  
  assertMatchPartial(triePtr, {1, 2, TRIE_TIMES});
  assertMatchPartial(triePtr, {1, 3, TRIE_TIMES});
  assertMatchPartial(triePtr, {1});
  assertMatchPartial(triePtr, {2});
  assertMatchPartial(triePtr, {3});

  assertNotMatch(triePtr, {2, 1, TRIE_PLUS});
  assertNotMatch(triePtr, {1, 2, TRIE_TIMES, 1, TRIE_PLUS});
  assertNotMatch(triePtr, {1, 2, TRIE_TIMES, 3, TRIE_PLUS});
  assertNotMatch(triePtr, {2, 1, 3, TRIE_TIMES, TRIE_PLUS});
  assertNotMatch(triePtr, {1, 1, 3, TRIE_TIMES, TRIE_PLUS});
}

TEST_CASE("Trie Building - Stress Tree") {
  PKB pkb;
  unique_ptr<PkbWriter> writer = make_unique<PkbWriter>(&pkb);
  auto exprTree = SourceParser().parseExpression(
      "call + procedure + assign - if + 20000 - read + call - read - if * print / while * 1000 + while + call + procedure + assign + if - while / 299385 - asdfasdf123123");
  auto trie = PatternConverter::convertASTToTrie(exprTree->getRoot(), writer.get());
}

