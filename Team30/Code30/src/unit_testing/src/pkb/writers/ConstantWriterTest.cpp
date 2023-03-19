#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/ConstantWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("ConstantWriter addConstant") {
  auto table = make_shared<EntityTable>();
  auto reverseTable = make_shared<EntityRevTable>();
  auto store = make_unique<ConstantStorage>(table.get(), reverseTable.get());
  auto writer = ConstantWriter(store.get());

  writer.addConstant("0");
  writer.addConstant("1");

  REQUIRE(store->getByKey(1) == "0");
  REQUIRE(store->getByKey(2) == "1");
  REQUIRE(store->getByValue("0") == unordered_set<int>({1}));
  REQUIRE(store->getByValue("1") == unordered_set<int>({2}));
}
