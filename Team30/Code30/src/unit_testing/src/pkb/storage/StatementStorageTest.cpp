#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::unordered_set;

TEST_CASE("StatementStorage addStatement") {
  StatementStorage store =
      StatementStorage(std::make_shared<ContiguousTable<StmtType>>(),
                       std::make_shared<HashKeySetTable<StmtType, int>>());
  store.insert(1, StmtType::Assign);
  store.insert(2, StmtType::Assign);
  store.insert(3, StmtType::Read);

  REQUIRE(store.getByValue(StmtType::Assign) == unordered_set<int>({1, 2}));
  REQUIRE(store.getByValue(StmtType::Read) == unordered_set<int>({3}));
}
