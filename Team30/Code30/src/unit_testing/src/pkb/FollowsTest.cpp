#include <unordered_set>
#include <memory>
#include "catch.hpp"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set;
using std::make_unique;

TEST_CASE("Follows") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 = handler.queryFollows({StmtType::None, 1}, {StmtType::None, 2});
  REQUIRE(result1.isEmpty == false);

  auto result2 = handler.queryFollows({StmtType::None, 1}, {StmtType::None, 3});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      handler.queryFollows({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result3.pairVals == pair_set<int, int>({{1, 2}}));

  auto result4 =
      handler.queryFollows({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result4.pairVals == pair_set<int, int>({{3, 4}}));

}

TEST_CASE("Follows 2 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      handler.queryFollows({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}}));

  auto result2 =
      handler.queryFollows({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}}));

  auto result5 =
      handler.queryFollows({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result5.pairVals == pair_set<int, int>({{3, 4}}));

  auto result6 =
      handler.queryFollows({StmtType::None, 0}, {StmtType::None, 4});
  REQUIRE(result6.pairVals == pair_set<int, int>({{3, 4}}));
}

TEST_CASE("FollowsStar <= 1 unknown") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      handler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 4});
  REQUIRE(result1.isEmpty == false);

  auto result2 =
      handler.queryFollowsStar({StmtType::None, 3}, {StmtType::None, 1});
  REQUIRE(result2.isEmpty == true);

  auto result3 =
      handler.queryFollowsStar({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result3.pairVals == pair_set<int, int>({{1, 2}, {1, 3}}));

  auto result4 =
      handler.queryFollowsStar({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result4.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));
}

TEST_CASE("FollowsStar 2 unknowns") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      handler.queryFollowsStar({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {1, 3}}));

  auto result2 =
      handler.queryFollowsStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.pairVals
              == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}, {1, 3}, {1, 4},
                                     {2, 4}}));

  auto result3 =
      handler.queryFollowsStar({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));
}
