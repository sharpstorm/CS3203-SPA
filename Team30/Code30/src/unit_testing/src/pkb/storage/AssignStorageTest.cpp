#include <set>
#include <memory>
#include <utility>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "pkb/storage/StorageTypes.h"
#include "qps/common/pattern/PatternConverter.h"
#include "sp/pattern/TrieBuilder.h"

using std::set, std::make_unique, std::shared_ptr;

TEST_CASE("AssignStorage addAssign") {
  PKB pkb;
  unique_ptr<PkbWriter> writer = make_unique<PkbWriter>(&pkb);
  // x = a + b
  auto node1 = make_unique<VariableASTNode>("a");
  auto node2 = make_unique<VariableASTNode>("b");
  auto node3 = make_unique<PlusASTNode>();
  node3->setChild(0, std::move(node1));
  node3->setChild(1, std::move(node2));

  auto trie = TrieBuilder(node3.get(), writer.get()).build();
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  AssignStorage store = AssignStorage();
  store.insert(1, sTrie);

  REQUIRE(store.get(1).get() == sTrie.get());
}
