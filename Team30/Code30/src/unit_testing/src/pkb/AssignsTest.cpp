#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/pattern/TrieBuilder.h"

using std::make_unique, std::make_shared;

TEST_CASE("Assign write and read") {
  auto pkb = make_unique<PKB>();
  PkbWriter writer = PkbWriter(pkb.get());
  PkbQueryHandler queryHandler = PkbQueryHandler(pkb.get());
  auto astRoot = make_unique<VariableASTNode>("a");
  PatternTrieSPtr node1 = TrieBuilder(astRoot.get(), &writer).build();

  writer.addAssigns(1, node1);
  auto result = *queryHandler.queryAssigns({StmtType::Assign, 1});

  REQUIRE(result.isEmpty == false);
  REQUIRE(*result.secondArgVals.begin() == node1.get());
}
