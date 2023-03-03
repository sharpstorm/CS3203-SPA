#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::string;
using std::unordered_set;

TEST_CASE("SymbolStorage addSymbol") {
  SymbolStorage store = SymbolStorage();
  store.set(EntityType::Variable, "x");
  store.set(EntityType::Variable, "y");
  store.set(EntityType::Constant, "0");
  store.set(EntityType::Constant, "1");
  store.set(EntityType::Procedure, "main");
  store.set(EntityType::Procedure, "x");

  REQUIRE(store.get(EntityType::Variable) ==
      unordered_set<string>({"x", "y"}));
  REQUIRE(store.get(EntityType::Constant) ==
      unordered_set<string>({"1", "0"}));
  REQUIRE(store.get(EntityType::Procedure) ==
      unordered_set<string>({"main", "x"}));
}
