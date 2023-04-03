#include <memory>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/StatementStorage.h"
#include "pkb/storage/StorageTypes.h"

using std::make_unique;

TEST_CASE("StatementStorage") {
  auto table = make_unique<StmtTable>();
  auto reverseTable = make_unique<StmtRevTable>();
  auto values = make_unique<StmtSet>();
  StatementStorage store(table.get(), reverseTable.get(), values.get());

  store.insert(1, StmtType::Assign);
  store.insert(2, StmtType::Assign);
  store.insert(3, StmtType::Read);

  REQUIRE(store.getStatementsOfType(StmtType::Assign) == StmtSet({1, 2}));
  REQUIRE(store.getStatementsOfType(StmtType::Read) == StmtSet({3}));
  REQUIRE(store.getStatementsOfType(StmtType::If) == StmtSet({}));
  REQUIRE(store.getStatementsOfType(StmtType::None) == StmtSet({1, 2, 3}));

  REQUIRE(store.getTypeOfStatement(2) == StmtType::Assign);
  REQUIRE(store.getTypeOfStatement(3) == StmtType::Read);

  REQUIRE(store.getAllStatements() == StmtSet({1, 2, 3}));
}
