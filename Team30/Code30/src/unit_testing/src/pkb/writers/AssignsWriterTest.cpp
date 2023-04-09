#include <memory>

#include "catch.hpp"
#include "pkb/writers/AssignsWriter.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/pattern/TrieBuilder.h"

using std::make_shared;
using std::make_unique, std::unique_ptr;

TEST_CASE("AssignWriter addAssigns") {
  PKB pkb;
  unique_ptr<PkbWriter> pkbWriter = make_unique<PkbWriter>(&pkb);
  auto store = make_unique<AssignStorage>();
  auto writer = AssignsWriter(store.get());
  auto astRoot = make_unique<VariableASTNode>("a");
  auto trie = TrieBuilder(astRoot.get(), pkbWriter.get()).build();
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));

  writer.addAssigns(1, sTrie);
  REQUIRE(store->get(1).get() == sTrie.get());
}
