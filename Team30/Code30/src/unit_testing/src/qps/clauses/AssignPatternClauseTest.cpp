#include "catch.hpp"

#include <memory>
#include <string>
#include <utility>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/arguments/EntityArgument.h"
#include "ClausesPKBStub.cpp"
#include "../util/SynonymHolder.h"
#include "../util/QueryResultTestUtil.cpp"
#include "sp/ast/expression_operand/PlusASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"
#include "qps/clauses/arguments/WildcardArgument.h"
#include "common/pattern/PatternConverter.h"
#include "sp/ast/AST.h"

using std::shared_ptr, std::make_shared, std::make_unique, std::unique_ptr, std::to_string;

/*
 * 1.    a = 1;
 * 2.    b = x;
 * 3.    a = 1 + 2;
 * 4.    b = y + x;
 * 5.    c = 2 + z + y;
 */

ASTNodePtr genPlus(ASTNodePtr left, ASTNodePtr right) {
  auto node = make_unique<PlusASTNode>();
  node->setLeftChild(std::move(left));
  node->setRightChild(std::move(right));
  return node;
}

ASTNodePtr genInteger(int val) {
  return make_unique<ConstantASTNode>(to_string(val));
}

ASTNodePtr genVariable(string val) {
  return make_unique<VariableASTNode>(val);
}

class AssignPatternPKBStub : public StubPKB {
 private:
  PatternTriePtr line1;
  PatternTriePtr line2;
  PatternTriePtr line3;
  PatternTriePtr line4;
  PatternTriePtr line5;
  PkbWriter* writer;

 public:
  AssignPatternPKBStub(PKB* in, PkbWriter* wr):
      StubPKB(in),
      line1(PatternConverter::convertASTToTrie(genInteger(1).get(), wr)),
      line2(PatternConverter::convertASTToTrie(genVariable("x").get(), wr)),
      line3(PatternConverter::convertASTToTrie(
          genPlus(std::move(genInteger(1)),
                  std::move(genInteger(2))).get(), wr)),
      line4(PatternConverter::convertASTToTrie(
          genPlus(genVariable("y"), genVariable("x")).get(), wr)),
      line5(PatternConverter::convertASTToTrie(genPlus(genPlus(genInteger(2),genVariable("z")),
                                                       genVariable("y")).get(), wr)) {
  }

  QueryResultPtr<int, string> queryModifies(StmtRef stmtRef, EntityRef entRef) const override {
    auto res = make_unique<QueryResult<int,string>>();
    if (stmtRef.isKnown()) {
      if (!entRef.isKnown()) {
        switch (stmtRef.getValue()) {
          case 1:
            res->add(1, "a");
            break;
          case 2:
            res->add(2, "b");
            break;
          case 3:
            res->add(3, "a");
            break;
          case 4:
            res->add(4, "b");
            break;
          case 5:
            res->add(5, "c");
            break;
        }
      } else if (stmtRef.getValue() == 1 && entRef.getValue() == "a") {
        res->add(1, "a");
      }

      return res;
    }

    if (!entRef.isKnown()) {
      res->add(1, "a");
      res->add(2, "b");
      res->add(3, "a");
      res->add(4, "b");
      res->add(5, "c");
    } else if (entRef.getValue() == "a") {
      res->add(1, "a");
      res->add(3, "a");
    } else if (entRef.getValue() == "b") {
      res->add(2, "b");
      res->add(4, "b");
    } else if (entRef.getValue() == "c") {
      res->add(5, "c");
    }

    return res;
  }

  QueryResultPtr<int, PatternTrie*> queryAssigns(StmtRef ref) const override {
    auto res = make_unique<QueryResult<int,PatternTrie*>>();
    switch (ref.getValue()) {
      case 1:
        res->add(1, line1.get());
        break;
      case 2:
        res->add(2, line2.get());
        break;
      case 3:
        res->add(3, line3.get());
        break;
      case 4:
        res->add(4, line4.get());
        break;
      case 5:
        res->add(5, line5.get());
        break;
    }

    return res;
  }

  StmtType getStatementType(int) const override {
    return StmtType::Assign;
  }

  bool isStatementOfType(StmtType type, int value) const override {
    return true;
  }

  bool isSymbolOfType(EntityType type, string symbol) const override {
    return true;
  }

};

IASTPtr makeIASTNode(ASTNodePtr node) {
  return make_unique<AST>(std::move(node));
}

SynonymHolder ASSIGN_PATTERN_SYNS({{PQL_SYN_TYPE_ASSIGN, "a"}, {PQL_SYN_TYPE_VARIABLE, "v"}});

TEST_CASE("Assign Pattern Constant-Exact") {
  PKB pkbStore;
  auto writer = make_unique<PkbWriter>(&pkbStore);
  auto pkb = make_unique<AssignPatternPKBStub>(&pkbStore, writer.get());

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  OverrideTablePtr override = make_unique<OverrideTable>();
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), override.get(), cache.get());

  // Constant-Variable-Exact
  ASTNodePtr node = make_unique<VariableASTNode>("x");
  PatternClausePtr patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<EntityArgument>("b"),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 2 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Integer-Exact
  node = make_unique<ConstantASTNode>("1");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<EntityArgument>("a"),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 1 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Constant-Wildcard") {
  PKB pkbStore;
  auto writer = make_unique<PkbWriter>(&pkbStore);
  auto pkb = make_unique<AssignPatternPKBStub>(&pkbStore, writer.get());

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;
  OverrideTablePtr override = make_unique<OverrideTable>();
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), override.get(), cache.get());

  // Constant-Wildcard
  PatternClausePtr patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<EntityArgument>("b"),
      IASTPtr(),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{2, 4});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Wildcard
  ASTNodePtr node = make_unique<VariableASTNode>("x");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<EntityArgument>("b"),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{2, 4});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Integer-Wildcard
  node = make_unique<ConstantASTNode>("1");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<EntityArgument>("a"),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{1, 3});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Variable-Exact") {
  PKB pkbStore;
  auto writer = make_unique<PkbWriter>(&pkbStore);
  auto pkb = make_unique<AssignPatternPKBStub>(&pkbStore, writer.get());

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  OverrideTablePtr table = make_unique<OverrideTable>();
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), table.get(), cache.get());;

  // Variable-Integer-Exact
  ASTNodePtr node = make_unique<ConstantASTNode>("1");
  PatternClausePtr patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{ 1, "a" }});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Exact
  node = make_unique<VariableASTNode>("x");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{ 2, "b" }});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Variable-Integer-Exact
  node = make_unique<ConstantASTNode>("1");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<WildcardArgument>(),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 1 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Exact
  node = make_unique<VariableASTNode>("x");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<WildcardArgument>(),
      makeIASTNode(std::move(node)),
      false);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 2 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern Variable-Partial") {
  PKB pkbStore;
  auto writer = make_unique<PkbWriter>(&pkbStore);
  auto pkb = make_unique<AssignPatternPKBStub>(&pkbStore, writer.get());

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  OverrideTablePtr table = make_unique<OverrideTable>();
  QueryCachePtr cache = make_unique<QueryCache>();
  QueryExecutorAgent agent(pkb.get(), table.get(), cache.get());

  // Variable-Integer-Partial
  ASTNodePtr node = make_unique<ConstantASTNode>("1");
  PatternClausePtr patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{
      { 1, "a" },
      { 3, "a" }
  });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Partial
  node = make_unique<VariableASTNode>("x");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{
      { 2, "b" },
      { 4, "b" }
  });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Variable-Integer-Partial
  node = make_unique<ConstantASTNode>("2");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<WildcardArgument>(),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 3, 5 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Constant-Variable-Partial
  node = make_unique<VariableASTNode>("y");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<WildcardArgument>(),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", unordered_set<int>{ 4, 5 });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);
}

TEST_CASE("Assign Pattern - with Clause") {
  PKB pkbStore;
  auto writer = make_unique<PkbWriter>(&pkbStore);
  auto pkb = make_unique<AssignPatternPKBStub>(&pkbStore, writer.get());
  QueryCachePtr cache = make_unique<QueryCache>();

  PQLQueryResultPtr expected;
  PQLQueryResultPtr actual;

  // with Clause on if
  OverrideTablePtr table = make_unique<OverrideTable>();
  table->insert("a", OverrideTransformer(1));
  QueryExecutorAgent agent(pkb.get(), table.get(), cache.get());

  ASTNodePtr node = make_unique<ConstantASTNode>("1");
  PatternClausePtr patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{
      { 1, "a" },
  });
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // with Clause on variable
  table = make_unique<OverrideTable>();
  table->insert("v", OverrideTransformer("a"));
  agent = QueryExecutorAgent(pkb.get(), table.get(), cache.get());
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      std::move(IASTPtr()),
      true
  );

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{1, "a"}, {3, "a"}});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // with Clause on if and variable
  table = make_unique<OverrideTable>();
  table->insert("a", OverrideTransformer(1));
  table->insert("v", OverrideTransformer("a"));
  node = make_unique<ConstantASTNode>("1");
  agent = QueryExecutorAgent(pkb.get(), table.get(), cache.get());
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      std::move(makeIASTNode(std::move(node))),
      true
  );

  expected = make_unique<PQLQueryResult>();
  expected->add("a", "v", pair_set<int, string>{{1, "a"}});
  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(*expected == *actual);

  // Negative with on if
  table = make_unique<OverrideTable>();
  table->insert("a", OverrideTransformer(6));
  agent = QueryExecutorAgent(pkb.get(), table.get(), cache.get());

  node = make_unique<ConstantASTNode>("1");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(actual->isFalse());

  // Valid if, invalid var
  table = make_unique<OverrideTable>();
  table->insert("a", OverrideTransformer(1));
  table->insert("v", OverrideTransformer("b"));
  agent = QueryExecutorAgent(pkb.get(), table.get(), cache.get());

  node = make_unique<ConstantASTNode>("1");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(actual->isFalse());

  // valid if, valid var, not matching trie
  table = make_unique<OverrideTable>();
  table->insert("a", OverrideTransformer(1));
  table->insert("v", OverrideTransformer("a"));
  agent = QueryExecutorAgent(pkb.get(), table.get(), cache.get());

  node = make_unique<ConstantASTNode>("99");
  patternClause = make_unique<AssignPatternClause>(
      ASSIGN_PATTERN_SYNS.getProxy("a"),
      make_unique<SynonymArgument>(ASSIGN_PATTERN_SYNS.getProxy("v")),
      makeIASTNode(std::move(node)),
      true);

  actual = PQLQueryResultPtr(patternClause->evaluateOn(agent));
  REQUIRE(actual->isFalse());

}
