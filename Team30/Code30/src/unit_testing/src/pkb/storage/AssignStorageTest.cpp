#include <unordered_set>
#include <memory>
#include <utility>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "pkb/storage/StorageTypes.h"
#include "common/pattern/PatternConverter.h"

using std::unordered_set, std::make_unique, std::shared_ptr;

TEST_CASE("AssignStorage addAssign") {
  // x = a + b
  auto node1 = make_unique<VariableASTNode>("a");
  auto node2 = make_unique<VariableASTNode>("b");
  auto node3 = make_unique<PlusASTNode>();
  node3->setChild(0, std::move(node1));
  node3->setChild(1, std::move(node2));

  auto trie = PatternConverter::convertASTToTrie(node3.get());
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  AssignStorage store = AssignStorage();
  store.set(1, sTrie);

  REQUIRE(store.get(1).get() == sTrie.get());
}
