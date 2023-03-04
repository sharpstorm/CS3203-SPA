#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "pkb/writers/AssignsWriter.h"

using std::make_shared;
using std::make_unique;
using std::unordered_set;

TEST_CASE("AssignWriter addAssigns") {
  auto store = make_unique<AssignStorage>();
  auto writer = AssignsWriter(store.get());
  PatternTrieSPtr node1 = make_shared<PatternTrie>(make_shared<VariableASTNode>("a"));
  writer.addAssigns(1, node1);
  REQUIRE(store->get(1) == node1);
}
