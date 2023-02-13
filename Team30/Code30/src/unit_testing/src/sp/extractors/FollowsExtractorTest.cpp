#include <common/ASTNode/math/ConditionalExpressionASTNode.h>

#include <memory>
#include <unordered_set>

#include "../../../../spa/src/pkb/storage/TransitiveRelationTableManager.h"
#include "../../../../spa/src/pkb/storage/tables/ContiguousSetTable.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

using std::make_shared;
using std::unordered_set;

class PkbWriterStubForFollows : public PkbWriter {
 public:
  TransitiveRelationTableManager<int>* storage;

  PkbWriterStubForFollows(PKB* pkb, TransitiveRelationTableManager<int>* store)
      : PkbWriter(pkb) {
    storage = store;
  }

  void addFollows(int arg1, int arg2) override { storage->insert(arg1, arg2); }
};

shared_ptr<StatementListNode> simplyLst() {
  shared_ptr<StatementListNode> simple =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> first =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> second =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> third =
      shared_ptr<StatementASTNode>(new PrintNode());

  first->lineNumber = 1;
  second->lineNumber = 2;
  third->lineNumber = 3;

  simple->addChild(first);
  simple->addChild(second);
  simple->addChild(third);
  return simple;
}

shared_ptr<StatementListNode> stmListWithWhile() {
  shared_ptr<StatementListNode> simple =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> first =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> second =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> third =
      shared_ptr<StatementASTNode>(new PrintNode());

  shared_ptr<WhileNode> simpleWhile = shared_ptr<WhileNode>(new WhileNode());

  shared_ptr<StatementListNode> stmtLst =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> firstWChild =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> secondWChild =
      shared_ptr<StatementASTNode>(new PrintNode());
  shared_ptr<StatementASTNode> thirdWChild =
      shared_ptr<StatementASTNode>(new AssignNode());

  shared_ptr<ConditionalExpressionASTNode> condition =
      shared_ptr<ConditionalExpressionASTNode>();

  first->lineNumber = 1;
  second->lineNumber = 2;
  simpleWhile->lineNumber = 3;
  firstWChild->lineNumber = 4;
  secondWChild->lineNumber = 5;
  thirdWChild->lineNumber = 6;
  third->lineNumber = 7;

  simple->addChild(first);
  simple->addChild(second);
  simple->addChild(third);
  stmtLst->addChild(first);
  stmtLst->addChild(second);
  stmtLst->addChild(third);

  simpleWhile->setChild(0, condition);
  simpleWhile->setChild(1, stmtLst);

  simple->addChild(simpleWhile);
  return simple;
}

TEST_CASE("FollowsExtractor simpleStmtLst") {
  shared_ptr<StatementListNode> simple = simplyLst();

  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = new TransitiveRelationTableManager<int>(table, reverseTable);
  PKB* pkb = new PKB();

  PkbWriterStubForFollows writer = PkbWriterStubForFollows(pkb, store);

  FollowsExtractor* extractor = new FollowsExtractor(&writer);

  extractor->visit(*simple);

  REQUIRE(table->get(1) == unordered_set<int>({2}));
  REQUIRE(reverseTable->get(2) == unordered_set<int>({1}));
  REQUIRE(table->get(2) == unordered_set<int>({3}));
  REQUIRE(reverseTable->get(3) == unordered_set<int>({2}));
}

TEST_CASE("FollowsExtractor Statement with While loop inbetween") {
  shared_ptr<StatementListNode> simple = simplyLst();

  auto table = make_shared<ContiguousSetTable<int>>();
  auto reverseTable = make_shared<ContiguousSetTable<int>>();
  auto store = new TransitiveRelationTableManager<int>(table, reverseTable);
  PKB* pkb = new PKB();

  PkbWriterStubForFollows writer = PkbWriterStubForFollows(pkb, store);

  FollowsExtractor* extractor = new FollowsExtractor(&writer);

  extractor->visit(*simple);

  REQUIRE(table->get(1) == unordered_set<int>({2}));
  REQUIRE(table->get(2) == unordered_set<int>({3}));
  REQUIRE(table->get(3) !=
          unordered_set<int>(
              {4}));  // statement 4 should not follow from statement 3
}
