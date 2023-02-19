#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "common/ASTNode/entity/VariableASTNode.h"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;
using std::unordered_set;

TEST_CASE("Assign write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());
  shared_ptr<IASTNode> node1 = shared_ptr<IASTNode>(new VariableASTNode("a"));

  writer.addAssigns(1, node1);
  auto result = queryHandler.queryAssigns({StmtType::Assign, 1});

  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(*result.secondArgVals.begin() == node1);
  REQUIRE(result.pairVals == pair_set<int, shared_ptr<IASTNode>>({{1, node1}}));
}
