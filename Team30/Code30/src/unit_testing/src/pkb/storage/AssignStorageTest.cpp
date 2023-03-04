#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "pkb/storage/StorageTypes.h"

using std::unordered_set, std::make_shared;

TEST_CASE("AssignStorage addAssign") {
  // x = a + b
  PatternTrieSPtr node1 = make_shared<PatternTrie>(make_shared<VariableASTNode>("a"));
  PatternTrieSPtr node2 = make_shared<PatternTrie>(make_shared<VariableASTNode>("b"));
  PatternTrieSPtr node3 = make_shared<PatternTrie>(make_shared<PlusASTNode>());
  AssignStorage store = AssignStorage();
  store.set(1, node1);
  store.set(2, node2);
  store.set(3, node3);

  REQUIRE(store.get(1) == node1);
  REQUIRE(store.get(2) == node2);
  REQUIRE(store.get(3) == node3);
}
