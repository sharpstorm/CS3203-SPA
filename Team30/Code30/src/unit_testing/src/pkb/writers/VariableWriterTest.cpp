#include <memory>

#include "catch.hpp"
#include "pkb/writers/VariableWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("VariableWriter addVariable") {
  auto table = make_shared<VarTable>();
  auto reverseTable = make_shared<VarRevTable>();
  auto values = make_shared<EntityValueSet>();
  auto store = make_unique<VariableStorage>(table.get(), reverseTable.get(),
                                            values.get());
  auto writer = VariableWriter(store.get());

  auto idx1 = writer.addVariable("x");
  auto idx2 = writer.addVariable("y");
  auto idx3 = writer.addVariable("x");
  REQUIRE(idx1 == 1);
  REQUIRE(idx2 == 2);
  REQUIRE(idx3 == 1);

  REQUIRE(store->getValueByIdx(1) == "x");
  REQUIRE(store->getValueByIdx(2) == "y");
  REQUIRE(store->getIdxOfValue("x") == 1);
  REQUIRE(store->getIdxOfValue("y") == 2);
}
