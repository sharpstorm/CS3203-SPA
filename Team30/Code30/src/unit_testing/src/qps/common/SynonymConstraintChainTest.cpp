#include "catch.hpp"
#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"
#include "qps/common/IConstraint.h"
#include "qps/constraints/SynonymConstraint.h"

bool isVarTableSynEqual(VariableTable *varTable,
                            const PQLSynonymName var1,
                            const PQLSynonymName var2) {
  auto syn1 = varTable->find(var1);
  auto syn2 = varTable->find(var2);
  REQUIRE(syn1 != nullptr);
  REQUIRE(syn2 != nullptr);

  auto syn1Proxy = *syn1;
  auto syn2Proxy = *syn2;
  return *syn1Proxy == *syn2Proxy;
}

TEST_CASE("SynonymConstraint Test") {
  PQLQuerySynonym syn1{PQL_SYN_TYPE_STMT, "s1"};
  PQLQuerySynonym syn2{PQL_SYN_TYPE_STMT, "s2"};
  PQLQuerySynonym syn3{PQL_SYN_TYPE_STMT, "s3"};
  PQLQuerySynonym syn4{PQL_SYN_TYPE_STMT, "s4"};
  PQLQuerySynonym syn5{PQL_SYN_TYPE_STMT, "s5"};

  VariableTable varTable;
  varTable.add(syn1);
  varTable.add(syn2);
  varTable.add(syn3);
  varTable.add(syn4);
  varTable.add(syn5);
  varTable.finalizeTable();

  SynonymProxyBuilder varTableProxyBuilder(varTable.getProxyMap());
  OverrideTable overrideTable;

  SynonymConstraint constraint1("s1", "s2");
  SynonymConstraint constraint2("s2", "s3");
  SynonymConstraint constraint3("s4", "s1");

  constraint1.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s2"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s1", "s3"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s2", "s3"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s1", "s4"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s2", "s4"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s3", "s4"));

  constraint2.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s2"));
  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s3"));
  REQUIRE(isVarTableSynEqual(&varTable, "s2", "s3"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s1", "s4"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s2", "s4"));
  REQUIRE_FALSE(isVarTableSynEqual(&varTable, "s3", "s4"));

  constraint3.applyConstraint(&varTableProxyBuilder, &overrideTable);
  varTableProxyBuilder.build();

  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s2"));
  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s3"));
  REQUIRE(isVarTableSynEqual(&varTable, "s2", "s3"));
  REQUIRE(isVarTableSynEqual(&varTable, "s1", "s4"));
  REQUIRE(isVarTableSynEqual(&varTable, "s2", "s4"));
  REQUIRE(isVarTableSynEqual(&varTable, "s3", "s4"));
}