#include <unordered_set>

#include "catch.hpp"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/math/math_operand/PlusASTNode.h"
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"

using std::unordered_set;

TEST_CASE("AssignStorage addAssign") {
  // x = a + b
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new VariableASTNode("a"));
  shared_ptr<IASTNode> node2 = shared_ptr<IASTNode>(new VariableASTNode("b"));
  shared_ptr<IASTNode> node3 = shared_ptr<IASTNode>(new PlusASTNode());
  AssignStorage store = AssignStorage();
  store.set(1, node1);
  store.set(2, node2);
  store.set(3, node3);

  REQUIRE(store.get(1) == node1);
  REQUIRE(store.get(2) == node2);
  REQUIRE(store.get(3) == node3);
}
