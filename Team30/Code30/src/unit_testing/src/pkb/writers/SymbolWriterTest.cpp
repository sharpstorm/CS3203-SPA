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
  auto store = make_unique<SymbolStorage>();
  auto writer = SymbolWriter(store.get());

  writer.addSymbol("x", EntityType::Variable);
  writer.addSymbol("y", EntityType::Variable);
  writer.addSymbol("0", EntityType::Constant);
  writer.addSymbol("1", EntityType::Constant);
  writer.addSymbol("main", EntityType::Procedure);
  writer.addSymbol("x", EntityType::Procedure);

  REQUIRE(store->get(EntityType::Variable) ==
      unordered_set<string>({"x", "y"}));
  REQUIRE(store->get(EntityType::Constant) ==
      unordered_set<string>({"0", "1"}));
  REQUIRE(store->get(EntityType::Procedure) ==
      unordered_set<string>({"x", "main"}));
}
