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
  SymbolStorage store = SymbolStorage(
      std::make_shared<HashKeyTable<std::string, EntityType>>(),
      std::make_shared<HashKeySetTable<EntityType, std::string>>());
  store.insert("x", EntityType::Variable);
  store.insert("y", EntityType::Variable);
  store.insert("0", EntityType::Constant);
  store.insert("1", EntityType::Constant);

  REQUIRE(store.getByValue(EntityType::Variable) ==
          unordered_set<string>({"x", "y"}));
  REQUIRE(store.getByValue(EntityType::Constant) ==
          unordered_set<string>({"1", "0"}));
}
