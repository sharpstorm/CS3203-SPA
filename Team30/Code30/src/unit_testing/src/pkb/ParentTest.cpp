#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/ParentQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::unordered_set;
using std::make_unique;

TEST_CASE("Parent") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 2);
  writer.addParent(2, 3);
  writer.addParent(3, 5);
  writer.addStatement(5, StmtType::Assign);

  auto result1 =
      queryHandler.queryParent({StmtType::None, 3}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{3, 5}}));
  auto result2 =
      queryHandler.queryParent({StmtType::Assign, 0}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);
}

TEST_CASE("ParentStar") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addParent(1, 3);
  writer.addParent(3, 5);
  writer.addParent(5, 6);
  writer.addParent(6, 7);
  writer.addParent(7, 8);
  writer.addStatement(5, StmtType::Assign);
  writer.addStatement(7, StmtType::Assign);

  auto result1 =
      queryHandler.queryParentStar({StmtType::None, 1}, {StmtType::Assign, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 5}, {1, 7}}));
}
