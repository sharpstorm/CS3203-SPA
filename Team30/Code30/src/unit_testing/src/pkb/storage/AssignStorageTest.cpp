#include <unordered_set>
#include <memory>
#include <utility>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "pkb/storage/StorageTypes.h"
#include "common/pattern/PatternConverter.h"

using std::unordered_set, std::make_shared, std::shared_ptr;

TEST_CASE("AssignStorage addAssign") {
  // x = a + b
  auto node1 = make_shared<VariableASTNode>("a");
  auto node2 = make_shared<VariableASTNode>("b");
  auto node3 = make_shared<PlusASTNode>();
  node3->setChild(0, node1);
  node3->setChild(1, node2);

  auto trie = PatternConverter::convertASTToTrie(node3);
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  AssignStorage store = AssignStorage();
  store.set(1, sTrie);

  REQUIRE(store.get(1).get() == sTrie.get());
}
