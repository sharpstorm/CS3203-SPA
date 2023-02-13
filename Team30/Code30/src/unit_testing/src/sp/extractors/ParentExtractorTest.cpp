#include <memory>
#include <unordered_set>
#include <vector>

#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/storage/StorageTypes.h"
#include "../../../../spa/src/pkb/storage/TransitiveRelationTableManager.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/common/ASTNode/StatementListNode.h"
#include "../../../../spa/src/sp/common/ASTNode/math/ConditionalExpressionASTNode.h"
#include "../../../../spa/src/sp/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/sp/common/ASTNode/statement/IfNode.h"
#include "../../../../spa/src/sp/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/sp/common/ASTNode/statement/StatementASTNode.h"
#include "../../../../spa/src/sp/common/ASTNode/statement/WhileNode.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/ParentExtractor.h"
#include "catch.hpp"
#include "pkb/storage/tables/ContiguousTable.h"

using std::make_shared;
using std::unordered_set;

class PkbWriterStubForParent : public PkbWriter {
 public:
  TransitiveRelationTableManager<int>* storage;

  PkbWriterStubForParent(PKB* pkb, TransitiveRelationTableManager<int>* store)
      : PkbWriter(pkb) {
    storage = store;
  }

  void addParent(int arg1, int arg2) override { storage->insert(arg1, arg2); }
};

shared_ptr<IfNode> simplyIf() {
  shared_ptr<IfNode> simpleIf = shared_ptr<IfNode>(new IfNode());

  shared_ptr<StatementListNode> ifLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementListNode> elseLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> first =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> second =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> third =
      shared_ptr<StatementASTNode>(new AssignNode());
  shared_ptr<StatementASTNode> fourth =
      shared_ptr<StatementASTNode>(new AssignNode());

  shared_ptr<ConditionalExpressionASTNode> condition =
      shared_ptr<ConditionalExpressionASTNode>(
          new ConditionalExpressionASTNode());

  simpleIf->lineNumber = 1;
  first->lineNumber = 2;
  second->lineNumber = 3;
  third->lineNumber = 4;
  fourth->lineNumber = 5;

  ifLst->addChild(first);
  ifLst->addChild(second);
  elseLst->addChild(third);
  elseLst->addChild(fourth);

  simpleIf->setChild(0, condition);
  simpleIf->setChild(1, ifLst);
  simpleIf->setChild(2, elseLst);

  return simpleIf;
}

shared_ptr<WhileNode> simplyWhile() {
  shared_ptr<WhileNode> simpleWhile = shared_ptr<WhileNode>(new WhileNode());

  shared_ptr<StatementListNode> stmtLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> first =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> second =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> third =
      shared_ptr<StatementASTNode>(new AssignNode());

  shared_ptr<ConditionalExpressionASTNode> condition =
      shared_ptr<ConditionalExpressionASTNode>();

  simpleWhile->lineNumber = 1;
  first->lineNumber = 2;
  second->lineNumber = 3;
  third->lineNumber = 4;

  stmtLst->addChild(first);
  stmtLst->addChild(second);
  stmtLst->addChild(third);

  simpleWhile->setChild(0, condition);
  simpleWhile->setChild(1, stmtLst);

  return simpleWhile;
}

shared_ptr<IfNode> ifWithWhile() {
  shared_ptr<IfNode> ifNode = shared_ptr<IfNode>(new IfNode());

  shared_ptr<StatementListNode> ifLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementListNode> elseLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> stmtIf =
      shared_ptr<StatementASTNode>(new PrintNode());

  shared_ptr<ConditionalExpressionASTNode> condition =
      shared_ptr<ConditionalExpressionASTNode>(
          new ConditionalExpressionASTNode());

  shared_ptr<WhileNode> whileNode = shared_ptr<WhileNode>(new WhileNode());

  shared_ptr<StatementListNode> stmtLstWhile =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> firstWhileStmt =
      shared_ptr<StatementASTNode>(new PrintNode());

  shared_ptr<ConditionalExpressionASTNode> conditionWhile =
      shared_ptr<ConditionalExpressionASTNode>();

  ifNode->lineNumber = 1;
  whileNode->lineNumber = 2;
  firstWhileStmt->lineNumber = 3;
  stmtIf->lineNumber = 4;

  stmtLstWhile->addChild(firstWhileStmt);

  whileNode->setChild(0, conditionWhile);
  whileNode->setChild(1, stmtLstWhile);

  ifLst->addChild(whileNode);
  elseLst->addChild(stmtIf);

  ifNode->setChild(0, condition);
  ifNode->setChild(1, ifLst);
  ifNode->setChild(2, elseLst);

  return ifNode;
}

TEST_CASE("ParentExtractor simple If") {
  shared_ptr<IfNode> simpleIf = simplyIf();

  auto table = make_shared<ContiguousTable<int>>();
  auto reverseTable = make_shared<ContiguousTable<int>>();
  auto store = new TransitiveRelationTableManager<int>(table, reverseTable);
  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb, store);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*simpleIf);

  REQUIRE(table->get(1) == unordered_set<int>({2, 3, 4, 5}));
  REQUIRE(reverseTable->get(2) == unordered_set<int>({1}));
  REQUIRE(reverseTable->get(3) == unordered_set<int>({1}));
  REQUIRE(reverseTable->get(4) == unordered_set<int>({1}));
  REQUIRE(reverseTable->get(5) == unordered_set<int>({1}));
}

TEST_CASE("ParentExtractor simple While") {
  shared_ptr<WhileNode> simpleWhile = simplyWhile();

  auto table = make_shared<ContiguousTable<int>>();
  auto reverseTable = make_shared<ContiguousTable<int>>();
  auto store = new TransitiveRelationTableManager<int>(table, reverseTable);
  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb, store);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*simpleWhile);

  REQUIRE(table->get(1) == unordered_set<int>({2, 3, 4}));
  REQUIRE(reverseTable->get(2) == unordered_set<int>({1}));
  REQUIRE(reverseTable->get(3) == unordered_set<int>({1}));
  REQUIRE(reverseTable->get(4) == unordered_set<int>({1}));
}

TEST_CASE("ParentExtractor if with while stmt") {
  shared_ptr<IfNode> ifNode = ifWithWhile();

  auto table = make_shared<ContiguousTable<int>>();
  auto reverseTable = make_shared<ContiguousTable<int>>();
  auto store = new TransitiveRelationTableManager<int>(table, reverseTable);
  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb, store);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*ifWithWhile());

  REQUIRE(table->get(1) == unordered_set<int>({2, 4}));
  REQUIRE(
      reverseTable->get(3) !=
      unordered_set<int>({1}));  // statement 3 should not have 1 as its parent
}
