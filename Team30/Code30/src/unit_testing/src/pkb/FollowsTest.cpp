#include <unordered_set>
#include <memory>
#include "catch.hpp"
#include "pkb/queryHandlers/FollowsQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/writers/FollowsWriter.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set;
using std::make_unique;

TEST_CASE("Follows") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  REQUIRE(queryHandler.queryFollows({StmtType::None, 1}, {StmtType::None, 2})
              .isEmpty == false);
  REQUIRE(queryHandler.queryFollows({StmtType::None, 1}, {StmtType::None, 3})
              .isEmpty == true);
  auto result1 =
      queryHandler.queryFollows({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}}));

  auto result2 =
      queryHandler.queryFollows({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{3, 4}}));

  auto result3 =
      queryHandler.queryFollows({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.pairVals == pair_set<int, int>({{3, 4}}));

  auto result4 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::None, 4});
  REQUIRE(result4.isEmpty == false);
  REQUIRE(result4.pairVals == pair_set<int, int>({{3, 4}}));

}

TEST_CASE("Follows follows(s,stmtType) or follows(s,s)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}}));

  auto result2 =
      queryHandler.queryFollows({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}}));

}

TEST_CASE("FollowsStar") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);
  writer.addFollows(4, 5);

  // writer.addSymbol("a", EntityType::Variable)
  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);
  writer.addStatement(5, StmtType::If);

  REQUIRE(
      queryHandler.queryFollowsStar({StmtType::None, 1}, {StmtType::None, 4})
          .isEmpty == false);
  REQUIRE(
      queryHandler.queryFollowsStar({StmtType::None, 3}, {StmtType::None, 1})
          .isEmpty == true);
  auto result1 =
      queryHandler.queryFollowsStar({StmtType::None, 1}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.firstArgVals == unordered_set<int>({1}));
  REQUIRE(result1.secondArgVals == unordered_set<int>({2, 3}));
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {1, 3}}));

  auto result2 =
      queryHandler.queryFollowsStar({StmtType::Read, 0}, {StmtType::None, 4});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));

  auto result3 =
      queryHandler.queryFollowsStar({StmtType::Read, 0}, {StmtType::Print, 0});
  REQUIRE(result3.isEmpty == false);
  REQUIRE(result3.pairVals == pair_set<int, int>({{2, 4}, {3, 4}}));
}

TEST_CASE("FollowsStar followsStar(s,stmtType) or followsStar(s,s)") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto queryHandler = PkbQueryHandler(pkb.get());

  writer.addFollows(1, 2);
  writer.addFollows(2, 3);
  writer.addFollows(3, 4);

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Read);
  writer.addStatement(3, StmtType::Read);
  writer.addStatement(4, StmtType::Print);

  auto result1 =
      queryHandler.queryFollowsStar({StmtType::None, 0}, {StmtType::Read, 0});
  REQUIRE(result1.isEmpty == false);
  REQUIRE(result1.pairVals == pair_set<int, int>({{1, 2}, {2, 3}, {1, 3}}));

  auto result2 =
      queryHandler.queryFollowsStar({StmtType::None, 0}, {StmtType::None, 0});
  REQUIRE(result2.isEmpty == false);
  REQUIRE(result2.pairVals
              == pair_set<int, int>({{1, 2}, {2, 3}, {3, 4}, {1, 3}, {1, 4},
                                     {2, 4}}));

}
