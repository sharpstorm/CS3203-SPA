#include <memory>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "common/pattern/PatternConverter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/ast/entity/VariableASTNode.h"

using std::make_unique, std::make_shared;
using std::unordered_set;

TEST_CASE("Assign write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());
  auto astRoot = make_unique<VariableASTNode>("a");
  PatternTrieSPtr node1 = PatternConverter::convertASTToTrie(astRoot.get());

  writer.addAssigns(1, node1);
  auto result = *queryHandler.queryAssigns({StmtType::Assign, 1});

  REQUIRE(result.isEmpty == false);
  REQUIRE(result.firstArgVals == unordered_set<int>({1}));
  REQUIRE(*result.secondArgVals.begin() == node1.get());
  REQUIRE(result.pairVals ==
          pair_set<int, PatternTrie *>({{1, node1.get()}}));
}
