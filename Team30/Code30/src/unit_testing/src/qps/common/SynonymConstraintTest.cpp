#include "catch.hpp"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"
#include "qps/common/constraint/Constraint.h"
#include "qps/common/constraint/SynonymConstraint.h"

TEST_CASE("SynonymConstraint Test") {
  PQLQuerySynonym syn1{PQL_SYN_TYPE_STMT, "s1"};
  PQLQuerySynonym syn2{PQL_SYN_TYPE_STMT, "s2"};
  PQLQuerySynonym syn3{PQL_SYN_TYPE_STMT, "s3"};
  PQLQuerySynonym syn4{PQL_SYN_TYPE_STMT, "s4"};
  PQLQuerySynonym syn5{PQL_SYN_TYPE_STMT, "s5"};

  VariableTable varTable;
  varTable.add("a", syn1);
  varTable.add("b", syn2);
  varTable.add("c", syn3);
  varTable.add("d", syn4);
  varTable.add("e", syn5);
  varTable.finalizeTable();

  PQLQuerySynonym* ptr1 = varTable.getProxyArray()->at("s1");
  PQLQuerySynonym* ptr2 = varTable.getProxyArray()->at("s2");
  PQLQuerySynonym* ptr3 = varTable.getProxyArray()->at("s3");
  PQLQuerySynonym* ptr4 = varTable.getProxyArray()->at("s4");
  PQLQuerySynonym* ptr5 = varTable.getProxyArray()->at("s5");

  VariableTableProxyBuilder varTableProxyBuilder(&varTable);
  OverrideTable overrideTable;

  SynonymConstraint constraint1(ptr1, ptr2);
  SynonymConstraint constraint2(ptr2, ptr3);
  SynonymConstraint constraint3(ptr4, ptr1);
  constraint1.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();


  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s2"));

  constraint2.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s2"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s3"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s2")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s3"));

  constraint3.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s2"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s3"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s2")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s3"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s1")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s4"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s2")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s4"));
  REQUIRE(varTableProxyBuilder.getTable()->getProxyArray()->at("s3")
              == varTableProxyBuilder.getTable()->getProxyArray()->at("s4"));
}