#include <memory>

#include "catch.hpp"
#include "pkb/writers/ConstantWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("ConstantWriter addConstant") {
  auto table = make_shared<ConstTable>();
  auto reverseTable = make_shared<ConstRevTable>();
  auto values = make_shared<EntityValueSet>();
  auto store = make_unique<ConstantStorage>(table.get(), reverseTable.get(),
                                            values.get());
  auto writer = ConstantWriter(store.get());

  auto idx1 = writer.addConstant("0");
  auto idx2 = writer.addConstant("1");
  auto idx3 = writer.addConstant("0");

  REQUIRE(idx1 == 1);
  REQUIRE(idx2 == 2);
  REQUIRE(idx3 == 1);

  REQUIRE(store->getValueByIdx(1) == "0");
  REQUIRE(store->getValueByIdx(2) == "1");
  REQUIRE(store->getIdxOfValue("0") == 1);
  REQUIRE(store->getIdxOfValue("1") == 2);
}
