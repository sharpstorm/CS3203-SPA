#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"

using std::unordered_set;

TEST_CASE("StatementStorage addStatement") {
  auto table = std::make_shared<StmtTable>();
  auto reverseTable = std::make_shared<StmtRevTable>();
  StatementStorage store = StatementStorage(table.get(), reverseTable.get());
  store.insert(1, StmtType::Assign);
  store.insert(2, StmtType::Assign);
  store.insert(3, StmtType::Read);

  REQUIRE(store.getByValue(StmtType::Assign) == unordered_set<int>({1, 2}));
  REQUIRE(store.getByValue(StmtType::Read) == unordered_set<int>({3}));
}
