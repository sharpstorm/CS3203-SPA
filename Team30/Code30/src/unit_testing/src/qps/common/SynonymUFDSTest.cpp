#include <iostream>
#include "catch.hpp"
#include "qps/common/VariableTable.h"
#include "qps/common/constraint/SynonymUFDS.h"

bool contains(vector<PQLQuerySynonym*>* v, PQLQuerySynonym* target) {
  return std::count(v->begin(), v->end(), target);
}

TEST_CASE("SynonymUFDS Test") {
  PQLQuerySynonym syn1{PQL_SYN_TYPE_STMT, "a"};
  PQLQuerySynonym syn2{PQL_SYN_TYPE_STMT, "b"};
  PQLQuerySynonym syn3{PQL_SYN_TYPE_STMT, "c"};
  PQLQuerySynonym syn4{PQL_SYN_TYPE_STMT, "d"};
  PQLQuerySynonym syn5{PQL_SYN_TYPE_STMT, "e"};

  VariableTable varTable;
  varTable.add("a", syn1);
  varTable.add("b", syn2);
  varTable.add("c", syn3);
  varTable.add("d", syn4);
  varTable.add("e", syn5);
  varTable.finalizeTable();

  PQLQuerySynonym* ptr1 = varTable.getProxyArray()->at("a");
  PQLQuerySynonym* ptr2 = varTable.getProxyArray()->at("b");
  PQLQuerySynonym* ptr3 = varTable.getProxyArray()->at("c");
  PQLQuerySynonym* ptr4 = varTable.getProxyArray()->at("d");
  PQLQuerySynonym* ptr5 = varTable.getProxyArray()->at("e");
  SynonymUFDS ufds(&varTable);

  vector<int> v = ufds.getParents();

  REQUIRE(v[0] == 0);
  REQUIRE(v[1] == 1);
  REQUIRE(v[2] == 2);
  REQUIRE(v[3] == 3);
  REQUIRE(v[4] == 4);


  REQUIRE(contains(&ufds.synArr,ptr1));
  REQUIRE(contains(&ufds.synArr,ptr2));
  REQUIRE(contains(&ufds.synArr,ptr3));
  REQUIRE(contains(&ufds.synArr,ptr4));
  REQUIRE(contains(&ufds.synArr,ptr5));

  REQUIRE(ufds.findSet(ptr1) == ptr1);
  REQUIRE(ufds.findSet(ptr2) == ptr2);
  REQUIRE(ufds.findSet(ptr3) == ptr3);
  REQUIRE(ufds.findSet(ptr4) == ptr4);
  REQUIRE(ufds.findSet(ptr5) == ptr5);

  ufds.mergeSets(ptr1, ptr2);
  REQUIRE(ufds.findSet(ptr1) == ufds.findSet(ptr2));

  ufds.mergeSets(ptr1, ptr3);
  REQUIRE(ufds.findSet(ptr2) == ufds.findSet(ptr3));
  REQUIRE(ufds.findSet(ptr1) == ufds.findSet(ptr3));

  ufds.mergeSets(ptr3, ptr4);
  REQUIRE(ufds.findSet(ptr1) == ufds.findSet(ptr4));
  REQUIRE(ufds.findSet(ptr2) == ufds.findSet(ptr4));
  REQUIRE(ufds.findSet(ptr3) == ufds.findSet(ptr4));
}