#include <common/ASTNode/math/ConditionalExpressionASTNode.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../../../../spa/src/pkb/storage/TransitiveRelationTableManager.h"
#include "../../../../spa/src/pkb/storage/tables/ContiguousSetTable.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "catch.hpp"
#include "sp/extractor/concrete_extractors/FollowsExtractor.h"

class PkbWriterStubForFollows : public PkbWriter {
 public:
  std::unordered_map<int, std::unordered_set<int>> relations;

  PkbWriterStubForFollows(PKB* pkb) : PkbWriter(pkb) {}

  void addFollows(int arg1, int arg2) final { relations[arg1].insert(arg2); };

  unordered_set<int> getSet(int index) {
    if (relations.count(index) == 1) {
      return relations[index];
    } else {
      return unordered_set<int>({});
    }
  }
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

  PKB* pkb = new PKB();

  PkbWriterStubForFollows writer = PkbWriterStubForFollows(pkb);

  FollowsExtractor* extractor = new FollowsExtractor(&writer);

  extractor->visit(*simple);

  REQUIRE(writer.getSet(1) == unordered_set<int>({2}));
  REQUIRE(writer.getSet(2) == unordered_set<int>({3}));
}

TEST_CASE("FollowsExtractor Statement with While loop inbetween") {
  shared_ptr<StatementListNode> simple = simplyLst();
  PKB* pkb = new PKB();

  PkbWriterStubForFollows writer = PkbWriterStubForFollows(pkb);

  FollowsExtractor* extractor = new FollowsExtractor(&writer);

  extractor->visit(*simple);

  REQUIRE(writer.getSet(1) == unordered_set<int>({2}));
  REQUIRE(writer.getSet(2) == unordered_set<int>({3}));
  REQUIRE(writer.getSet(3) !=
          unordered_set<int>(
              {4}));  // statement 4 should not follow from statement 3
}
