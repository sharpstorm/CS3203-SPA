#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/StatementWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("StatementWriter addStatement") {
  auto table = make_shared<HashKeyTable<int, StmtType>>();
  auto reverseTable = make_shared<HashKeySetTable<StmtType, int>>();
  auto store = make_unique<StatementStorage>(table, reverseTable);
  auto writer = StatementWriter(store.get());

  writer.addStatement(1, StmtType::Assign);
  writer.addStatement(2, StmtType::Assign);
  writer.addStatement(3, StmtType::Read);

  REQUIRE(table->get(1) == StmtType::Assign);
  REQUIRE(table->get(3) == StmtType::Read);
  REQUIRE(reverseTable->get(StmtType::Assign) == unordered_set<int>({1, 2}));
  REQUIRE(reverseTable->get(StmtType::Read) == unordered_set<int>({3}));
}
