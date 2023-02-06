#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/StatementStorage.h"

using std::unordered_set;

TEST_CASE("StatementStorage addStatement") {
  StatementStorage store = StatementStorage();
  store.addStatement(1, StmtType::Assign);
  store.addStatement(2, StmtType::Assign);
  store.addStatement(3, StmtType::Read);

  REQUIRE(store.getStatementsOfType(StmtType::Assign) ==
          unordered_set<int>({1, 2}));
  REQUIRE(store.getStatementsOfType(StmtType::Read) == unordered_set<int>({3}));
}