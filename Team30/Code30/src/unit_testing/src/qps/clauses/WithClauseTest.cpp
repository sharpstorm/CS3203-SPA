#include "catch.hpp"
#include "qps/clauses/WithClause.h"

/*
 * procedure p1 {
 *   call p3;
 *   print p2;
 * }
 *
 * procedure p2 {
 *   read p1;
 * }
 *
 * procedure p3 {
 *   call p2;
 *   call p2;
 * }
 *
 */

class WithStubPkb: public PkbQueryHandler {
 public:
  WithStubPkb(PKB* in): PkbQueryHandler(in) {
  }

  unordered_set<int> getStatementsOfType(StmtType type) const override {
    if (type == StmtType::Call) {
      return {1, 4, 5};
    } else if (type == StmtType::None) {
      return {1, 2, 3, 4, 5};
    } else if (type == StmtType::Print) {
      return {2};
    } else if (type == StmtType::Read) {
      return {3};
    }
    return {};
  };

  unordered_set<string> getSymbolsOfType(EntityType type) const override {
    if (type == EntityType::Variable) {
      return {"p1", "p2"};
    } else if (type == EntityType::Procedure) {
      return {"p1", "p2", "p3"};
    }
    return {};
  }

  string getCalledDeclaration(int stmt) const override {
    if (stmt == 1) {
      return "p3";
    } else if (stmt == 4 || stmt == 5) {
      return "p2";
    }
    return "";
  }

  string getReadDeclarations(int stmt) const override {
    if (stmt == 3) {
      return "p1";
    }
    return "";
  }

  string getPrintDeclarations(int stmt) const override {
    if (stmt == 2) {
      return "p2";
    }
    return "";
  }
};

PQLQuerySynonym makeSyn(PQLSynonymType type, PQLSynonymName name) {
  return PQLQuerySynonym(type, name);
}

AttributedSynonymPtr makeWithArg(PQLQuerySynonym* syn, PQLSynonymAttribute attr) {
  return make_unique<AttributedSynonym>(syn, attr);
}

PQLQueryResult* testWithClause(PQLSynonymType leftType, PQLSynonymName leftName, PQLSynonymAttribute leftAttr,
                               PQLSynonymType rightType, PQLSynonymName rightName, PQLSynonymAttribute rightAttr) {
  auto leftSyn = makeSyn(leftType, leftName);
  auto rightSyn = makeSyn(rightType, rightName);

  auto leftArg = make_unique<WithArgument>(std::move(makeWithArg(&leftSyn, leftAttr)));
  auto rightArg = make_unique<WithArgument>(std::move(makeWithArg(&rightSyn, rightAttr)));
  auto clause = WithClause(std::move(leftArg), std::move(rightArg));

  PKB pkb;
  WithStubPkb stubHandler(&pkb);
  OverrideTable table;
  auto result = clause.evaluateOn(&stubHandler, &table);
  return result;
}

TEST_CASE("With Execution Typing") {
  unique_ptr<PQLQueryResult> expected;
  PQLQueryResult* result;

  // int, int
  // default - default
  result = testWithClause(
      PQL_SYN_TYPE_CALL, "cl", STMT_NUM,
      PQL_SYN_TYPE_STMT, "s", STMT_NUM);
  expected = make_unique<PQLQueryResult>();
  expected->add("cl", "s", pair_set<int, int>{
      {1, 1},
      {4, 4},
      {5, 5}
  });

  REQUIRE(*result == *expected);
  delete result;

  // int - int
  // not-default - not default
  result = testWithClause(
      PQL_SYN_TYPE_PRINT, "p", VAR_NAME,
      PQL_SYN_TYPE_CALL, "cl", PROC_NAME);
  expected = make_unique<PQLQueryResult>();
  expected->add("p", "cl", pair_set<int, int>{
      {2, 4},
      {2, 5}
  });

  REQUIRE(*result == *expected);
  delete result;

  // string, int
  // default - non default
  result = testWithClause(
      PQL_SYN_TYPE_PROCEDURE, "p", PROC_NAME,
      PQL_SYN_TYPE_CALL, "cl", PROC_NAME);
  expected = make_unique<PQLQueryResult>();
  expected->add("p", "cl", pair_set<string, int>{
      {"p2", 4},
      {"p2", 5},
      {"p3", 1}
  });

  REQUIRE(*result == *expected);
  delete result;

  // int, string
  // non-default - default
  result = testWithClause(
      PQL_SYN_TYPE_CALL, "cl", PROC_NAME,
      PQL_SYN_TYPE_PROCEDURE, "p", PROC_NAME);
  expected = make_unique<PQLQueryResult>();
  expected->add("p", "cl", pair_set<string, int>{
      {"p2", 4},
      {"p2", 5},
      {"p3", 1}
  });

  REQUIRE(*result == *expected);
  delete result;

  result = testWithClause(
      PQL_SYN_TYPE_PROCEDURE, "p", PROC_NAME,
      PQL_SYN_TYPE_READ, "r", VAR_NAME);
  expected = make_unique<PQLQueryResult>();
  expected->add("p", "r", pair_set<string, int>{
      {"p1", 3}
  });

  REQUIRE(*result == *expected);
  delete result;

  // default - default
  // string - string
  result = testWithClause(
      PQL_SYN_TYPE_PROCEDURE, "p", PROC_NAME,
      PQL_SYN_TYPE_VARIABLE, "v", VAR_NAME);
  expected = make_unique<PQLQueryResult>();
  expected->add("p", "v", pair_set<string, string>{
      {"p1", "p1"},
      {"p2", "p2"}
  });

  REQUIRE(*result == *expected);
  delete result;
}
