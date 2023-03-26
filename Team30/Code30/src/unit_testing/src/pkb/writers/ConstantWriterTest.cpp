#include <memory>

#include "catch.hpp"
#include "pkb/writers/ConstantWriter.h"

using std::make_shared;
using std::make_unique;

TEST_CASE("ConstantWriter addConstant") {
  auto table = make_shared<ConstTable>();
  auto reverseTable = make_shared<ConstRevTable>();
  auto store = make_unique<ConstantStorage>(table.get(), reverseTable.get());
  auto writer = ConstantWriter(store.get());

  writer.addConstant("0");
  writer.addConstant("1");

  REQUIRE(store->getValueByIdx(1) == "0");
  REQUIRE(store->getValueByIdx(2) == "1");
  REQUIRE(store->getIdxOfValue("0") == 1);
  REQUIRE(store->getIdxOfValue("1") == 2);
}
