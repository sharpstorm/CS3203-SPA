#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/VariableWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("VariableWriter addVariable") {
  auto table = make_shared<EntityTable>();
  auto reverseTable = make_shared<EntityRevTable>();
  auto store = make_unique<VariableStorage>(table.get(), reverseTable.get());
  auto writer = VariableWriter(store.get());

  writer.addVariable("x");
  writer.addVariable("y");

  REQUIRE(store->getByKey(1) == "x");
  REQUIRE(store->getByKey(2) == "y");
  REQUIRE(store->getByValue("x") == unordered_set<int>({1}));
  REQUIRE(store->getByValue("y") == unordered_set<int>({2}));
}
