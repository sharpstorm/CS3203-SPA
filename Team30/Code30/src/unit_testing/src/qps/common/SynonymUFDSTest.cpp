#include <iostream>
#include "catch.hpp"
#include "qps/common/VariableTable.h"
#include "qps/common/constraint/SynonymUFDS.h"

bool contains(vector<PQLQuerySynonym*>* v, PQLQuerySynonym* target) {
  return std::count(v->begin(), v->end(), target);
}

class SynonymUFDSSpy : public SynonymUFDS {
 public:
  SynonymUFDSSpy(VariableTable* table) : SynonymUFDS(table) {}
  vector<UFDSSet> getParentsTesting() const {
    return getParents();
  }
};

TEST_CASE("SynonymUFDS Test") {
  PQLQuerySynonym syn1{PQL_SYN_TYPE_STMT, "a"};
  PQLQuerySynonym syn2{PQL_SYN_TYPE_STMT, "b"};
  PQLQuerySynonym syn3{PQL_SYN_TYPE_STMT, "c"};
  PQLQuerySynonym syn4{PQL_SYN_TYPE_STMT, "d"};
  PQLQuerySynonym syn5{PQL_SYN_TYPE_STMT, "e"};

  VariableTable varTable;
  varTable.add(syn1);
  varTable.add(syn2);
  varTable.add(syn3);
  varTable.add(syn4);
  varTable.add(syn5);
  varTable.finalizeTable();
  SynonymUFDSSpy ufds(&varTable);

  vector<int> v = ufds.getParentsTesting();

  REQUIRE(v[0] == 0);
  REQUIRE(v[1] == 1);
  REQUIRE(v[2] == 2);
  REQUIRE(v[3] == 3);
  REQUIRE(v[4] == 4);

  REQUIRE(*ufds.getSynFor("a") == syn1);
  REQUIRE(*ufds.getSynFor("b") == syn2);
  REQUIRE(*ufds.getSynFor("c") == syn3);
  REQUIRE(*ufds.getSynFor("d") == syn4);
  REQUIRE(*ufds.getSynFor("e") == syn5);


  ufds.tryMergeSets("a", "b");
  REQUIRE(ufds.getSynFor("a") == ufds.getSynFor("b"));

  ufds.tryMergeSets("c", "d");
  REQUIRE(ufds.getSynFor("c") == ufds.getSynFor("d"));

  ufds.tryMergeSets("d", "e");
  REQUIRE(ufds.getSynFor("c") == ufds.getSynFor("d"));
  REQUIRE(ufds.getSynFor("c") == ufds.getSynFor("e"));
  REQUIRE(ufds.getSynFor("d") == ufds.getSynFor("e"));

  ufds.tryMergeSets("a", "e");
  REQUIRE(ufds.getSynFor("a") == ufds.getSynFor("b"));
  REQUIRE(ufds.getSynFor("a") == ufds.getSynFor("c"));
  REQUIRE(ufds.getSynFor("a") == ufds.getSynFor("d"));
  REQUIRE(ufds.getSynFor("a") == ufds.getSynFor("e"));
}