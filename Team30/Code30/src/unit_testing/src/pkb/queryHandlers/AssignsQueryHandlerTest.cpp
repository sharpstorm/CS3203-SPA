#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "common/Types.h"
#include "pkb/queryHandlers/AssignsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "common/pattern/PatternConverter.h"

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
  auto test = assignTestInit();
  // x = a

  PatternTriePtr trie = PatternConverter::convertASTToTrie(
      make_shared<VariableASTNode>("a"));
  auto sTrie = shared_ptr<PatternTrie>(std::move(trie));
  test.store->set(1, sTrie);

  auto result = test.handler.queryAssigns({StmtType::Assign, 1});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(*result.secondArgVals.begin() == sTrie.get());
  REQUIRE(result.pairVals == pair_set<int, PatternTrie*>({{1, sTrie.get()}}));
}
