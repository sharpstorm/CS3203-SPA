#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "common/pattern/PatternConverter.h"
#include "pkb/queryHandlers/AssignsQueryHandler.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

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
  PatternTriePtr trie = PatternConverter::convertASTToTrie(root.get(), writer.get());
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  test.store->set(1, sTrie);

  auto result = *test.handler.queryAssigns({StmtType::Assign, 1});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(*result.secondArgVals.begin() == sTrie.get());
  REQUIRE(result.pairVals ==
          pair_set<int, PatternTrie *>({{1, sTrie.get()}}));
}
