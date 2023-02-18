#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/math/math_operand/PlusASTNode.h"
#include "common/Types.h"
#include "pkb/queryHandlers/AssignsQueryHandler.h"
#include "pkb/storage/StructureMappingProvider.h"

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
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new VariableASTNode("a"));
  test.store->set(1, node1);

  auto result = test.handler.queryAssigns({StmtType::Assign, 1});
  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(*result.secondArgVals.begin() == node1);
  REQUIRE(result.pairVals == pair_set<int, shared_ptr<IASTNode>>({{1, node1}}));
}
