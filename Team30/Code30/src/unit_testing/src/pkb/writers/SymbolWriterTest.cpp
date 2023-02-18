#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/writers/SymbolWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("SymbolWriter addSymbol") {
  auto table = make_shared<HashKeyTable<string, EntityType>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityType, string>>();
  auto store = make_unique<SymbolStorage>(table, reverseTable);
  auto writer = SymbolWriter(store.get());

  writer.addSymbol("x", EntityType::Variable);
  writer.addSymbol("y", EntityType::Variable);
  writer.addSymbol("0", EntityType::Constant);
  writer.addSymbol("1", EntityType::Constant);

  REQUIRE(table->get("x") == EntityType::Variable);
  REQUIRE(table->get("0") == EntityType::Constant);
  REQUIRE(reverseTable->get(EntityType::Variable) ==
          unordered_set<string>({"x", "y"}));
  REQUIRE(reverseTable->get(EntityType::Constant) ==
          unordered_set<string>({"0", "1"}));
}
