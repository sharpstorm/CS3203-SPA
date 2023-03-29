#include "catch.hpp"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"
#include "qps/constraints/Constraint.h"
#include "qps/constraints/SynonymConstraint.h"

TEST_CASE("SynonymConstraint Test") {
  PQLQuerySynonym syn1{PQL_SYN_TYPE_STMT, "s1"};
  PQLQuerySynonym syn2{PQL_SYN_TYPE_STMT, "s2"};
  PQLQuerySynonym syn3{PQL_SYN_TYPE_STMT, "s3"};
  PQLQuerySynonym syn4{PQL_SYN_TYPE_STMT, "s4"};
  PQLQuerySynonym syn5{PQL_SYN_TYPE_STMT, "s5"};

  VariableTable varTable;
  varTable.add("s1", syn1);
  varTable.add("s2", syn2);
  varTable.add("s3", syn3);
  varTable.add("s4", syn4);
  varTable.add("s5", syn5);
  varTable.finalizeTable();

  SynonymProxyBuilder varTableProxyBuilder(&varTable);
  OverrideTable overrideTable;

  SynonymConstraint constraint1("s1", "s2");
  SynonymConstraint constraint2("s2", "s3");
  SynonymConstraint constraint3("s4", "s1");
  constraint1.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s2"));

  constraint2.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s2"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s3"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s2")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s3"));

  constraint3.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s2"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s3"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s2")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s3"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s1")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s4"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s2")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s4"));
  REQUIRE(*varTableProxyBuilder.getTable()->getProxyMap()->at("s3")
              == *varTableProxyBuilder.getTable()->getProxyMap()->at("s4"));
}