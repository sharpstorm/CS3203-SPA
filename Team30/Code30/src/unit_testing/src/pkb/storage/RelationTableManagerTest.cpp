#include <memory>
#include <utility>

#include "catch.hpp"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/tables/ContiguousSetTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

using std::make_shared;

TEST_CASE("RelationTableManager insert and getByArg1, getByArg2") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "b");

  REQUIRE(table->get(1) == EntityValueSet({"a", "c"}));
  REQUIRE(reverseTable->get("b") == StmtValueSet({2, 4}));
}

TEST_CASE("RelationTableManager query known arg1 values with arg2 predicate") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(1, "c");
  tableManager.insert(4, "e");
  tableManager.insert(3, "f");

  EntityValueSet validValues({"a", "e", "f"});
  Predicate<EntityValue> isValid = [validValues](const EntityValue s) -> bool {
    return validValues.find(s) != validValues.end();
  };

  auto resultBuilder = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder.setAllVals();
  auto res = *tableManager.query({1, 2, 4}, isValid, &resultBuilder);

  REQUIRE(res.firstArgVals == StmtValueSet({1, 4}));
  REQUIRE(res.secondArgVals == EntityValueSet({"a", "e"}));
  REQUIRE(res.pairVals ==
          pair_set<StmtValue, EntityValue>({{1, "a"}, {4, "e"}}));
}

TEST_CASE("RelationTableManager query known arg2 values with arg1 predicate") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");
  tableManager.insert(2, "c");
  tableManager.insert(3, "f");

  StmtValueSet validValues({2, 4, 5});
  Predicate<StmtValue> isValid = [validValues](StmtValue const &s) {
    return validValues.find(s) != validValues.end();
  };

  auto resultBuilder = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder.setAllVals();
  auto res = *tableManager.query(isValid, {"a", "b", "f"}, &resultBuilder);

  REQUIRE(res.firstArgVals == StmtValueSet({2, 4}));
  REQUIRE(res.secondArgVals == EntityValueSet({"a", "b"}));
  REQUIRE(res.pairVals ==
          pair_set<StmtValue, EntityValue>({{4, "a"}, {2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg1 with arg2 predicate") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(2, "c");

  EntityValueSet validValues({"a", "b"});
  Predicate<EntityValue> isValid = [validValues](EntityValue const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto resultBuilder = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder.setAllVals();
  auto res = *tableManager.query(2, isValid, &resultBuilder);

  REQUIRE(res.firstArgVals == StmtValueSet({2}));
  REQUIRE(res.secondArgVals == EntityValueSet({"b"}));
  REQUIRE(res.pairVals == pair_set<StmtValue, EntityValue>({{2, "b"}}));
}

TEST_CASE("RelationTableManager query known arg2 with arg1 predicate") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(2, "b");
  tableManager.insert(4, "a");

  StmtValueSet validValues({2, 4});
  Predicate<StmtValue> isValid = [validValues](StmtValue const &s) {
    return validValues.find(s) != validValues.end();
  };
  auto resultBuilder = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder.setAllVals();
  auto res = *tableManager.query(isValid, "a", &resultBuilder);

  REQUIRE(res.firstArgVals == StmtValueSet({4}));
  REQUIRE(res.secondArgVals == EntityValueSet({"a"}));
  REQUIRE(res.pairVals == pair_set<StmtValue, EntityValue>({{4, "a"}}));
}

TEST_CASE("RelationTableManager query both known args") {
  auto table = make_shared<ContiguousSetTable<EntityValue>>();
  auto reverseTable = make_shared<HashKeySetTable<EntityValue, StmtValue>>();
  RelationTableManager<StmtValue, EntityValue> tableManager(table.get(),
                                                            reverseTable.get());

  tableManager.insert(1, "a");
  tableManager.insert(1, "b");
  tableManager.insert(1, "c");
  tableManager.insert(2, "a");

  auto resultBuilder1 = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder1.setAllVals();
  auto res1 = *tableManager.query(1, "b", &resultBuilder1);
  REQUIRE(res1.isEmpty == false);
  REQUIRE(res1.pairVals == pair_set<StmtValue, EntityValue>({{1, "b"}}));

  auto resultBuilder2 = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder2.setAllVals();
  auto res2 = *tableManager.query(2, "b", &resultBuilder2);
  REQUIRE(res2.isEmpty == true);

  auto resultBuilder3 = QueryResultBuilder<StmtValue, EntityValue>();
  resultBuilder3.setAllVals();
  auto res3 = *tableManager.query(3, "a", &resultBuilder3);
  REQUIRE(res3.isEmpty == true);
}
