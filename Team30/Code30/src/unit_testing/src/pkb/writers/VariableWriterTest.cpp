#include <memory>
#include <string>
#include <unordered_set>

#include "catch.hpp"
#include "pkb/writers/VariableWriter.h"

using std::make_shared;
using std::make_unique;
using std::string;
using std::unordered_set;

TEST_CASE("VariableWriter addVariable") {
  auto table = make_shared<VarTable>();
  auto reverseTable = make_shared<VarRevTable>();
  auto store = make_unique<VariableStorage>(table.get(), reverseTable.get());
  auto writer = VariableWriter(store.get());

  writer.addVariable("x");
  writer.addVariable("y");

  REQUIRE(store->getValueByIdx(1) == "x");
  REQUIRE(store->getValueByIdx(2) == "y");
  REQUIRE(store->getIdxOfValue("x") == 1);
  REQUIRE(store->getIdxOfValue("y") == 2);
}
