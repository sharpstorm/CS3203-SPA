#include "catch.hpp"
#include "common/data_structs/AppendSet.h"

class AppendSetSpy : public AppendSet<int> {
 public:
  vector<int> &getItems() {
    return items;
  }
};

TEST_CASE("Append Set Add to End") {
  AppendSetSpy set;
  set.insert(1);
  set.insert(2);
  set.insert(6);
  set.insert(123456);

  REQUIRE(set.getItems() == vector<int>{1,2,6,123456});
  REQUIRE(set.find(1) != set.end());
  REQUIRE(set.find(2) != set.end());
  REQUIRE(set.find(6) != set.end());
  REQUIRE(set.find(123456) != set.end());

  REQUIRE(set.find(0) == set.end());
  REQUIRE(set.find(4) == set.end());
  REQUIRE(set.find(653) == set.end());
  REQUIRE(set.find(-123) == set.end());
  REQUIRE(set.find(2147483647) == set.end());
}

TEST_CASE("Append Set Add in middle - Expect rearrange") {
  AppendSetSpy set;
  set.insert(1);
  set.insert(2);
  set.insert(6);
  set.insert(123456);
  REQUIRE(set.getItems() == vector<int>{1,2,6,123456});

  // Middle
  REQUIRE(set.find(5) == set.end());
  set.insert(5);
  REQUIRE(set.getItems() == vector<int>{1,2,5,6,123456});
  REQUIRE(set.find(5) != set.end());

  REQUIRE(set.find(23454) == set.end());
  set.insert(23454);
  REQUIRE(set.getItems() == vector<int>{1,2,5,6,23454,123456});
  REQUIRE(set.find(23454) != set.end());

  // Head
  REQUIRE(set.find(0) == set.end());
  set.insert(0);
  REQUIRE(set.getItems() == vector<int>{0,1,2,5,6,23454,123456});
  REQUIRE(set.find(0) != set.end());
}



