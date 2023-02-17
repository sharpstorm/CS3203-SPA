#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set;
using std::make_unique;

TEST_CASE("Modifies (StmtRef, EntityRef)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addModifies(3, "x");
  writer.addModifies(4, "x");
  writer.addModifies(4, "y");
  writer.addModifies(1, "x");
  writer.addModifies(1, "y");
  writer.addModifies(2, "x");
  writer.addModifies(2, "y");
  writer.addStatement(1, StmtType::While);
  writer.addStatement(2, StmtType::If);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Assign);
  writer.addSymbol("x", EntityType::Variable);
  writer.addSymbol("y", EntityType::Variable);
  writer.addSymbol("z", EntityType::Variable);

  auto result1 =
      handler.queryModifies({StmtType::None, 1}, {EntityType::None, "y"});
  REQUIRE(result1.pairVals == pair_set<int, string>({{1, "y"}}));

  auto result2 =
      handler.queryModifies({StmtType::None, 1}, {EntityType::Variable, ""});
  REQUIRE(result2.pairVals == pair_set<int, string>({{1, "y"}, {1, "x"}}));

  auto result3 =
      handler.queryModifies({StmtType::Assign, 0}, {EntityType::None, "x"});
  REQUIRE(result3.pairVals == pair_set<int, string>({{4, "x"}}));

  auto result4 =
      handler.queryModifies({StmtType::None, 0}, {EntityType::Variable, ""});
  REQUIRE(result4.pairVals
              == pair_set<int, string>({{1, "x"}, {1, "y"}, {2, "x"}, {2, "y"},
                                        {3, "x"}, {4, "x"}, {4, "y"},}));
}

