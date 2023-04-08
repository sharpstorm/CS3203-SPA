#include "catch.hpp"

#include <memory>
#include <utility>

#include "qps/common/VariableTable.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/common/constraint/SynonymProxyBuilder.h"
#include "../util/SynonymHolder.h"
#include "qps/constraints/OverrideConstraint.h"

using std::unordered_map, std::make_unique;

typedef unordered_map<PQLSynonymName, PQLQuerySynonym> OverrideMap;

OverrideMap OVERRIDE_MAP({
                             {"a", {PQL_SYN_TYPE_ASSIGN, "a"}},
                             {"v", {PQL_SYN_TYPE_VARIABLE, "v"}},
                             {"cl", {PQL_SYN_TYPE_CALL, "cl"}},
                             {"p", {PQL_SYN_TYPE_PROCEDURE, "p"}},
                             {"c", {PQL_SYN_TYPE_CONSTANT, "c"}},
                             {"pn", {PQL_SYN_TYPE_PRINT, "pn"}},
                             {"r", {PQL_SYN_TYPE_READ, "r"}}
                         });

SynonymHolder OVERRIDE_SYNS(
    {
        {PQL_SYN_TYPE_ASSIGN, "a"},
        {PQL_SYN_TYPE_VARIABLE, "v"},
        {PQL_SYN_TYPE_CALL, "cl"},
        {PQL_SYN_TYPE_PROCEDURE, "p"},
        {PQL_SYN_TYPE_CONSTANT, "c"},
        {PQL_SYN_TYPE_PRINT, "pn"},
        {PQL_SYN_TYPE_READ, "r"}
    });

TEST_CASE("OverrideConstraint - Get affected Syns") {
  VariableTable varTable;
  SynonymProxyBuilder builder(varTable.getProxyMap());
  OverrideTable overrides;

  AttributedSynonym syn(OVERRIDE_SYNS.getProxy("a"), STMT_NUM);
  OverrideConstraint constraint(syn, 1);
  vector<PQLSynonymName> expected = vector<PQLSynonymName>({"a"});
  REQUIRE(expected == constraint.getAffectedSyns());
}

TEST_CASE("OverrideConstraint - Validate Constraint Success") {
  VariableTable varTable;
  SynonymProxyBuilder builder(varTable.getProxyMap());
  OverrideTable overrides;

  AttributedSynonym syn(OVERRIDE_SYNS.getProxy("a"), STMT_NUM);
  OverrideConstraint constraint(syn, 1);
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("v"), VAR_NAME);
  constraint = OverrideConstraint(syn, "x");
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("r"), VAR_NAME);
  constraint = OverrideConstraint(syn, "readVar");
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("pn"), VAR_NAME);
  constraint = OverrideConstraint(syn, "printVar");
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("p"), PROC_NAME);
  constraint = OverrideConstraint(syn, "proc");
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("cl"), PROC_NAME);
  constraint = OverrideConstraint(syn, "callProc");
  REQUIRE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("c"), CONST_VALUE);
  constraint = OverrideConstraint(syn, 5);
  REQUIRE(constraint.validateConstraint());
}

TEST_CASE("OverrideConstraint - Validate Constraint Failure") {
  VariableTable varTable;
  SynonymProxyBuilder builder(varTable.getProxyMap());
  OverrideTable overrides;

  AttributedSynonym syn(OVERRIDE_SYNS.getProxy("a"), STMT_NUM);
  OverrideConstraint constraint(syn, "x");
  REQUIRE_FALSE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("v"), VAR_NAME);
  constraint = OverrideConstraint(syn, 5);
  REQUIRE_FALSE(constraint.validateConstraint());

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("v"), NO_ATTRIBUTE);
  constraint = OverrideConstraint(syn, "x");
  REQUIRE_THROWS(constraint.validateConstraint());
}

TEST_CASE("OverrideConstraint - Apply Constraint Success") {
  VariableTable varTable;
  SynonymProxyBuilder builder(varTable.getProxyMap());
  OverrideTable overrides;

  AttributedSynonym syn(OVERRIDE_SYNS.getProxy("a"), STMT_NUM);
  OverrideConstraint constraint(syn, 1);
  // Unique entry
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  // Duplicate but same value
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("v"), VAR_NAME);
  constraint = OverrideConstraint(syn, "x");
  // Unique entry
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  // Duplicate but same value
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("p"), PROC_NAME);
  constraint = OverrideConstraint(syn, "proc");
  // Unique entry
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  // Duplicate but same value
  REQUIRE(constraint.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("c"), CONST_VALUE);
  constraint = OverrideConstraint(syn, 7);
  // Unique entry
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  // Duplicate but same value
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
}

TEST_CASE("OverrideConstraint - Apply Constraint Failure") {
  VariableTable varTable;
  SynonymProxyBuilder builder(varTable.getProxyMap());
  OverrideTable overrides;

  AttributedSynonym syn(OVERRIDE_SYNS.getProxy("a"), STMT_NUM);
  OverrideConstraint constraint(syn, 1);
  OverrideConstraint constraintFail(syn, 3);
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  REQUIRE_FALSE(constraintFail.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("v"), VAR_NAME);
  constraint = OverrideConstraint(syn, "x");
  constraintFail = OverrideConstraint(syn, "y");
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  REQUIRE_FALSE(constraintFail.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("p"), PROC_NAME);
  constraint = OverrideConstraint(syn, "proc");
  constraintFail = OverrideConstraint(syn, "procFail");
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  REQUIRE_FALSE(constraintFail.applyConstraint(&builder, &overrides));

  syn = AttributedSynonym(OVERRIDE_SYNS.getProxy("c"), CONST_VALUE);
  constraint = OverrideConstraint(syn, 7);
  constraintFail = OverrideConstraint(syn, 9);
  REQUIRE(constraint.applyConstraint(&builder, &overrides));
  REQUIRE_FALSE(constraintFail.applyConstraint(&builder, &overrides));
}