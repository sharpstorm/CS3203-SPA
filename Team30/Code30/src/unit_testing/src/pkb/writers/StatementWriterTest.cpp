#include <memory>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/StatementWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("StatementWriter addStatement") {
  auto table = make_shared<StmtTable>();
  auto reverseTable = make_shared<StmtRevTable>();
  auto stmtValues = make_shared<StmtValueSet>();
  auto store = make_unique<StatementStorage>(table.get(), reverseTable.get(),
                                             stmtValues.get());
  auto writer = StatementWriter(store.get());

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Assign);
  writer.addStatement(3, StmtType::Read);

  REQUIRE(table->get(1) == StmtType::Assign);
  REQUIRE(table->get(3) == StmtType::Read);
  REQUIRE(reverseTable->get(StmtType::Assign) == StmtValueSet({1, 2}));
  REQUIRE(reverseTable->get(StmtType::Read) == StmtValueSet({3}));
}
