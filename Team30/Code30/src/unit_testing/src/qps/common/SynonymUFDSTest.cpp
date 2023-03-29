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
  SynonymUFDS ufds(&varTable);

  vector<int> v = ufds.getParents();

  REQUIRE(v[0] == 0);
  REQUIRE(v[1] == 1);
  REQUIRE(v[2] == 2);
  REQUIRE(v[3] == 3);
  REQUIRE(v[4] == 4);

  REQUIRE(*ufds.getSetValue("a") == syn1);
  REQUIRE(*ufds.getSetValue("b") == syn2);
  REQUIRE(*ufds.getSetValue("c") == syn3);
  REQUIRE(*ufds.getSetValue("d") == syn4);
  REQUIRE(*ufds.getSetValue("e") == syn5);


  ufds.tryMergeSets("a", "b");
  REQUIRE(ufds.getSetValue("a") == ufds.getSetValue("b"));

  ufds.tryMergeSets("c", "d");
  REQUIRE(ufds.getSetValue("c") == ufds.getSetValue("d"));

  ufds.tryMergeSets("d", "e");
  REQUIRE(ufds.getSetValue("c") == ufds.getSetValue("d"));
  REQUIRE(ufds.getSetValue("c") == ufds.getSetValue("e"));
  REQUIRE(ufds.getSetValue("d") == ufds.getSetValue("e"));

  ufds.tryMergeSets("a", "e");
  REQUIRE(ufds.getSetValue("a") == ufds.getSetValue("b"));
  REQUIRE(ufds.getSetValue("a") == ufds.getSetValue("c"));
  REQUIRE(ufds.getSetValue("a") == ufds.getSetValue("d"));
  REQUIRE(ufds.getSetValue("a") == ufds.getSetValue("e"));
}