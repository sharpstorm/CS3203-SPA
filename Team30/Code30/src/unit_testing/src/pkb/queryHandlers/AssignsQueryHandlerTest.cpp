#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/AssignsQueryHandler.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/pattern/TrieBuilder.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

struct assignTestInit {
  unique_ptr<AssignStorage> store;
  AssignsQueryHandler handler;

  assignTestInit()
      : store(make_unique<AssignStorage>()),
        handler(AssignsQueryHandler(store.get())) {}
};

// assign stmt
TEST_CASE("AssignQueryHandler Assigns(stmtRef)") {
  PKB pkb;
  unique_ptr<PkbWriter> writer = make_unique<PkbWriter>(&pkb);
  auto test = assignTestInit();
  // x = a

  auto root = make_unique<VariableASTNode>("a");
  PatternTriePtr trie = TrieBuilder(root.get(), writer.get()).build();
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  test.store->insert(1, sTrie);

  auto result = *test.handler.queryAssigns({StmtType::Assign, 1});
  REQUIRE(result.empty() == false);
  REQUIRE(*result.getRightVals().begin() == sTrie.get());
}
