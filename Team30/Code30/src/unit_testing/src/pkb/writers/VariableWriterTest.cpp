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
  auto table = make_shared<ContiguousTable<string >>();
  auto reverseTable = make_shared<HashKeySetTable<string, int>>();
  auto store = make_unique<VariableStorage>(table, reverseTable);
  auto writer = VariableWriter(store.get());

  writer.addVariable("x");
  writer.addVariable("y");
//  writer.addSymbol("0", EntityType::Constant);
//  writer.addSymbol("1", EntityType::Constant);
//  writer.addSymbol("main", EntityType::Procedure);
//  writer.addSymbol("x", EntityType::Procedure);

  REQUIRE(store->getByKey(1) == "x");
  REQUIRE(store->getByKey(2) == "y");
  REQUIRE(store->getByValue("x") == unordered_set<int>({1}));
  REQUIRE(store->getByValue("y") == unordered_set<int>({2}));
}
