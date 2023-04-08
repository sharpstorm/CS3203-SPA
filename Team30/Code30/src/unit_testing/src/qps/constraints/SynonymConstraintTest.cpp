#include "catch.hpp"
#include "qps/common/VariableTable.h"
#include "qps/common/constraint/SynonymProxyBuilder.h"
#include "qps/constraints/SynonymConstraint.h"

VariableTable buildVarTable(unordered_map<PQLSynonymName, PQLSynonymType> syns) {
  VariableTable ret;

  for (const auto &x : syns) {
    ret.add(PQLQuerySynonym(x.second, x.first));
  }

  return ret;
}

TEST_CASE("Synonym Constraint - Equal Level") {
  vector<PQLSynonymType> types = {
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_ASSIGN
  };

  for (auto type : types) {
    VariableTable varTable = buildVarTable(
        {
            {"s1", type},
            {"s2", type},
        });
    varTable.finalizeTable();
    SynonymProxyBuilder builder(&varTable);
    OverrideTable overrides;

    SynonymConstraint constraint("s1", "s2");
    REQUIRE(constraint.validateConstraint());
    REQUIRE(constraint.applyConstraint(&builder, &overrides));

    builder.build();
    REQUIRE(varTable.find("s1") != nullptr);
    REQUIRE(varTable.find("s2") != nullptr);
    REQUIRE(varTable.find("s1")->get() == varTable.find("s2")->get());
    REQUIRE((*varTable.find("s1"))->isType(type));
  }
}

TEST_CASE("Synonym Constraint - Subtype Specialization") {
  vector<PQLSynonymType> subtypes = {
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_ASSIGN
  };

  for (auto st : subtypes) {
    for (size_t i = 0; i < 2; i++) {
      auto s1Type = (i == 0) ? PQL_SYN_TYPE_STMT : st;
      auto s2Type = (i == 0) ? st : PQL_SYN_TYPE_STMT;
      auto narrower = (i == 0) ? "s2" : "s1";
      VariableTable varTable = buildVarTable(
          {
              {"s1", s1Type},
              {"s2", s2Type},
          });
      varTable.finalizeTable();
      SynonymProxyBuilder builder(&varTable);
      OverrideTable overrides;

      SynonymConstraint constraint("s1", "s2");
      REQUIRE(constraint.validateConstraint());
      REQUIRE(constraint.applyConstraint(&builder, &overrides));
      builder.build();
      REQUIRE(varTable.find("s1") != nullptr);
      REQUIRE(varTable.find("s2") != nullptr);
      REQUIRE(varTable.find("s1")->get() == varTable.find("s2")->get());
      REQUIRE((*varTable.find("s1"))->getName() == narrower);
    }
  }
}

TEST_CASE("Synonym Constraint - Chained Specialization") {
  VariableTable varTable = buildVarTable(
      {
          {"s1", PQL_SYN_TYPE_STMT},
          {"a2", PQL_SYN_TYPE_ASSIGN},
          {"a3", PQL_SYN_TYPE_ASSIGN},
      });
  varTable.finalizeTable();
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  SynonymConstraint constraint("s1", "a2");
  REQUIRE(constraint.validateConstraint());
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  constraint = SynonymConstraint("s1", "a3");
  REQUIRE(constraint.validateConstraint());
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  builder.build();
  REQUIRE(varTable.find("s1") != nullptr);
  REQUIRE(varTable.find("a2") != nullptr);
  REQUIRE(varTable.find("a3") != nullptr);
  REQUIRE(varTable.find("s1")->get() == varTable.find("a2")->get());
  REQUIRE(varTable.find("s1")->get() == varTable.find("a3")->get());
  REQUIRE((*varTable.find("s1"))->isType(PQL_SYN_TYPE_ASSIGN));
}

TEST_CASE("Synonym Constraint - Impossible Specialization") {
  VariableTable varTable = buildVarTable(
      {
          {"a2", PQL_SYN_TYPE_ASSIGN},
          {"w3", PQL_SYN_TYPE_WHILE},
      });
  varTable.finalizeTable();
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  SynonymConstraint constraint("a2", "w3");
  REQUIRE(constraint.validateConstraint());
  REQUIRE_FALSE(constraint.applyConstraint(&builder, &overrides));
}

TEST_CASE("Synonym Constraint - Impossible Specialization Chained") {
  VariableTable varTable = buildVarTable(
      {
          {"s1", PQL_SYN_TYPE_STMT},
          {"a2", PQL_SYN_TYPE_ASSIGN},
          {"w3", PQL_SYN_TYPE_WHILE},
      });
  varTable.finalizeTable();
  SynonymProxyBuilder builder(&varTable);
  OverrideTable overrides;

  SynonymConstraint constraint("s1", "a2");
  REQUIRE(constraint.validateConstraint());
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  constraint = SynonymConstraint("s1", "w3");
  REQUIRE(constraint.validateConstraint());
  REQUIRE_FALSE(constraint.applyConstraint(&builder, &overrides));
}
