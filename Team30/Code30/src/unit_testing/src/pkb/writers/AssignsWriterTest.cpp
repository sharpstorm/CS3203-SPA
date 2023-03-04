#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "pkb/writers/AssignsWriter.h"
#include "common/pattern/PatternConverter.h"

using std::make_shared;
using std::make_unique;
using std::unordered_set;

TEST_CASE("AssignWriter addAssigns") {
  auto store = make_unique<AssignStorage>();
  auto writer = AssignsWriter(store.get());
  auto trie = PatternConverter::convertASTToTrie(
      make_shared<VariableASTNode>("a"));
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));

  writer.addAssigns(1, sTrie);
  REQUIRE(store->get(1).get() == sTrie.get());
}
