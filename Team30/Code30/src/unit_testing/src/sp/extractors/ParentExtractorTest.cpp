#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../../../../spa/src/common/ASTNode/StatementListNode.h"
#include "../../../../spa/src/common/ASTNode/math/ConditionalExpressionASTNode.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/storage/StorageTypes.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/ParentExtractor.h"
#include "catch.hpp"

using std::make_shared;

class PkbWriterStubForParent : public PkbWriter {
 public:
  std::unordered_map<int, std::unordered_set<int>> relations;

  PkbWriterStubForParent(PKB* pkb) : PkbWriter(pkb) {}

  void addParent(int arg1, int arg2) final { relations[arg1].insert(arg2); };

  unordered_set<int> getSet(int index) {
    if (relations.count(index) == 1) {
      return relations[index];
    } else {
      return unordered_set<int>({});
    }
  }
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

  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*simpleIf);

  REQUIRE(writer.getSet(1) == unordered_set<int>({2, 3, 4, 5}));
  REQUIRE(writer.getSet(2) == unordered_set<int>({}));
  REQUIRE(writer.getSet(3) == unordered_set<int>({}));
  REQUIRE(writer.getSet(4) == unordered_set<int>({}));
  REQUIRE(writer.getSet(5) == unordered_set<int>({}));
}

TEST_CASE("ParentExtractor simple While") {
  shared_ptr<WhileNode> simpleWhile = simplyWhile();

  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*simpleWhile);

  REQUIRE(writer.getSet(1) == unordered_set<int>({
                                  2,
                                  3,
                                  4,
                              }));
  REQUIRE(writer.getSet(2) == unordered_set<int>({}));
  REQUIRE(writer.getSet(3) == unordered_set<int>({}));
  REQUIRE(writer.getSet(4) == unordered_set<int>({}));
}

TEST_CASE("ParentExtractor if with while stmt") {
  shared_ptr<IfNode> ifNode = ifWithWhile();

  PKB* pkb = new PKB();

  PkbWriterStubForParent writer = PkbWriterStubForParent(pkb);

  ParentExtractor* extractor = new ParentExtractor(&writer);

  extractor->visit(*ifWithWhile());

  REQUIRE(writer.getSet(1) == unordered_set<int>({2, 4}));
  REQUIRE(writer.getSet(2) == unordered_set<int>({3}));
  REQUIRE(writer.getSet(3) == unordered_set<int>({}));
}
