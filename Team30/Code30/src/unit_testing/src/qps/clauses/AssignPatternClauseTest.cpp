#include "catch.hpp"

#include <memory>
#include <string>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/QueryResultTestUtil.cpp"
#include "common/ASTNode/math/math_operand/PlusASTNode.h"
#include "common/ASTNode/entity/ConstantASTNode.h"
#include "common/ASTNode/entity/VariableASTNode.h"
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/arguments/WildcardArgument.h"

using std::shared_ptr, std::make_shared, std::make_unique, std::unique_ptr, std::to_string;

/*
 * 1.    a = 1;
 * 2.    b = x;
 * 3.    a = 1 + 2;
 * 4.    b = y + x;
 * 5.    c = 2 + z + y;
 */

shared_ptr<ASTNode> genPlus(shared_ptr<ASTNode> left,
                             shared_ptr<ASTNode> right) {
  auto node = make_shared<PlusASTNode>();
  node->setLeftChild(left);
  node->setRightChild(right);
  return node;
}

shared_ptr<ASTNode> genInteger(int val) {
  return make_shared<ConstantASTNode>(to_string(val));
}

shared_ptr<ASTNode> genVariable(string val) {
  return make_shared<VariableASTNode>(val);
}

class AssignPatternPKBStub : public StubPKB {
 public:
  AssignPatternPKBStub(PKB* in): StubPKB(in) {
  }

  QueryResult<int, string> queryModifies(StmtRef, EntityRef entRef) const override {
    QueryResult<int, string> res;
    if (!entRef.isKnown()) {
      res.add(1, "a");
      res.add(2, "b");
      res.add(3, "a");
      res.add(4, "b");
      res.add(5, "c");
    } else if (entRef.name == "a") {
      res.add(1, "a");
      res.add(3, "a");
    } else if (entRef.name == "b") {
      res.add(2, "b");
      res.add(4, "b");
    } else if (entRef.name == "c") {
      res.add(5, "c");
    }

    return res;
  }

  QueryResult<int, shared_ptr<IASTNode>> queryAssigns(StmtRef ref) const override {
    QueryResult<int, shared_ptr<IASTNode>> res;
    switch (ref.lineNum) {
      case 1:
        res.add(1, genInteger(1));
        break;
      case 2:
        res.add(2, genVariable("x"));
        break;
      case 3:
        res.add(3, genPlus(genInteger(1), genInteger(2)));
        break;
      case 4:
        res.add(4, genPlus(genVariable("y"), genVariable("x")));
        break;
      case 5:
        res.add(5, genPlus(genPlus(genInteger(2),genVariable("z")),
                           genVariable("y")));
        break;
    }

    return res;
  }
};

TEST_CASE("Assign Pattern Constant-Exact") {
  PKB pkbStore;
  shared_ptr<PkbQueryHandler> pkb = make_shared<AssignPatternPKBStub>(&pkbStore);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  PQLQuerySynonym assignSyn(PQL_SYN_TYPE_ASSIGN, "a");

  // Constant-Variable-Exact
  AssignPatternClause patternClause(
      assignSyn,
      ClauseArgumentPtr(new EntityArgument("b")),
      "x", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 2 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Integer-Exact
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new EntityArgument("a")),
      "1", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 1 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Constant-Wildcard") {
  PKB pkbStore;
  shared_ptr<PkbQueryHandler> pkb = make_shared<AssignPatternPKBStub>(&pkbStore);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  PQLQuerySynonym assignSyn(PQL_SYN_TYPE_ASSIGN, "a");

  // Constant-Wildcard
  AssignPatternClause patternClause(
      assignSyn,
      ClauseArgumentPtr(new EntityArgument("b")),
      "", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{2, 4});
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Wildcard
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new EntityArgument("b")),
      "x", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{2, 4});
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Integer-Wildcard
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new EntityArgument("a")),
      "1", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{1, 3});
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Variable-Exact") {
  PKB pkbStore;
  shared_ptr<PkbQueryHandler> pkb = make_shared<AssignPatternPKBStub>(&pkbStore);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  PQLQuerySynonym assignSyn(PQL_SYN_TYPE_ASSIGN, "a");
  PQLQuerySynonym varSyn(PQL_SYN_TYPE_VARIABLE, "v");

  // Variable-Integer-Exact
  AssignPatternClause patternClause(
      assignSyn,
      ClauseArgumentPtr(new SynonymArgument(varSyn)),
      "1", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{ 1, "a" }});
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Exact
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new SynonymArgument(varSyn)),
      "x", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{ 2, "b" }});
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Variable-Integer-Exact
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new WildcardArgument()),
      "1", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 1 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Exact
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new WildcardArgument()),
      "x", false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 2 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Variable-Partial") {
  PKB pkbStore;
  shared_ptr<PkbQueryHandler> pkb = make_shared<AssignPatternPKBStub>(&pkbStore);

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  PQLQuerySynonym assignSyn(PQL_SYN_TYPE_ASSIGN, "a");
  PQLQuerySynonym varSyn(PQL_SYN_TYPE_VARIABLE, "v");

  // Variable-Integer-Partial
  AssignPatternClause patternClause(
      assignSyn,
      ClauseArgumentPtr(new SynonymArgument(varSyn)),
      "1", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{
    { 1, "a" },
    { 3, "a" }
  });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Partial
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new SynonymArgument(varSyn)),
      "x", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{
    { 2, "b" },
    { 4, "b" }
  });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Variable-Integer-Partial
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new WildcardArgument()),
      "2", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 3, 5 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Partial
  patternClause = AssignPatternClause(
      assignSyn,
      ClauseArgumentPtr(new WildcardArgument()),
      "y", true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 4, 5 });
  actual = PQLQueryResultPtr(patternClause.evaluateOn(pkb));
  REQUIRE(*expected == *actual);
}
