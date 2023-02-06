#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/storage/SymbolStorage.h"

using std::string;
using std::unordered_set;

TEST_CASE("SymbolStorage addSymbol") {
  SymbolStorage store = SymbolStorage();
  store.addSymbol("x", EntityType::Variable);
  store.addSymbol("y", EntityType::Variable);
  store.addSymbol("0", EntityType::Constant);
  store.addSymbol("1", EntityType::Constant);

  REQUIRE(store.getSymbolsOfType(EntityType::Variable) ==
          unordered_set<string>({"x", "y"}));
  REQUIRE(store.getSymbolsOfType(EntityType::Constant) ==
          unordered_set<string>({"1", "0"}));
}