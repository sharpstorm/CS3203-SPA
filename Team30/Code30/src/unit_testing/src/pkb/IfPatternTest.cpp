#include <unordered_set>
#include <memory>

#include "catch.hpp"
#include "pkb/queryHandlers/IfPatternQueryHandler.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "pkb/storage/PKB.h"
#include "pkb/writers/PkbWriter.h"

using std::unordered_set, std::make_unique;

TEST_CASE("IfPattern") {
  auto pkb = make_unique<PKB>();
  auto writer = PkbWriter(pkb.get());
  auto handler = PkbQueryHandler(pkb.get());

  writer.addIfPattern(1, "a");
  writer.addIfPattern(1, "b");
  writer.addIfPattern(2, "a");
  writer.addIfPattern(3, "c");

  auto res1 = handler.queryIfPattern({EntityType::None, "a"});
  REQUIRE(res1.firstArgVals == unordered_set<int>({1, 2}));
  REQUIRE(res1.secondArgVals == unordered_set<string>({"a"}));
  REQUIRE(res1.pairVals == pair_set<int, string>({{1, "a"}, {2, "a"}}));

  auto res2 = handler.queryIfPattern({EntityType::Variable, ""});
  REQUIRE(res2.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res2.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res2.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));

  auto res3 = handler.queryIfPattern({EntityType::None, ""});
  REQUIRE(res3.firstArgVals == unordered_set<int>({1, 2, 3}));
  REQUIRE(res3.secondArgVals == unordered_set<string>({"a", "b", "c"}));
  REQUIRE(res3.pairVals == pair_set<int, string>({{1, "a"}, {1, "b"}, {2, "a"},
                                                  {3, "c"}}));
}
