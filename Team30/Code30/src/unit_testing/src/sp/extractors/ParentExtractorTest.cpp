#include <memory>
#include <unordered_set>
#include <vector>

#include "../../../../spa/src/pkb/writers/ParentWriter.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/common/ASTNode/StatementListNode.h"
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

class PkbWriterStub : public PkbWriter {
 public:
  FollowsWriter followsWriterStub;
  ParentWriter parentWriterStub;
  SymbolWriter symbolWriterStub;
  StatementWriter statementWriterStub;
  ProcedureWriter procedureWriterStub;

  PkbWriterStub(PKB* pkb, ParentWriter writer)
      : PkbWriter(pkb),
        followsWriterStub(pkb->followsStore),
        parentWriterStub(pkb->parentStore),
        symbolWriterStub(pkb->symbolStorage),
        statementWriterStub(pkb->statementStorage),
        procedureWriterStub(pkb->procedureStorage) {
    parentWriterStub = writer;
  }

  void addParent(int arg1, int arg2) override {
    parentWriterStub.addParent(arg1, arg2);
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

  first->lineNumber = 1;
  second->lineNumber = 2;
  third->lineNumber = 3;
  fourth->lineNumber = 4;

  ifLst->addChild(first);
  ifLst->addChild(second);
  elseLst->addChild(third);
  elseLst->addChild(fourth);

  simpleIf->addChild(ifLst);
  simpleIf->addChild(elseLst);

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

  first->lineNumber = 1;
  second->lineNumber = 2;
  third->lineNumber = 3;

  stmtLst->addChild(first);
  stmtLst->addChild(second);
  stmtLst->addChild(third);

  simpleWhile->addChild(stmtLst);

  return simpleWhile;
}

// TEST_CASE("ParentExtractor simple If") {
//  shared_ptr<IfNode> simple = simplyIf();
//
//  auto table = make_shared<ContiguousTable<int>>();
//  auto reverseTable = make_shared<ContiguousTable<int>>();
//  auto store = new FollowsStorage(table, reverseTable);
//  ParentWriter writerAccessible = ParentWriter(store);
//  PKB* pkb = new PKB();
//
//  PkbWriterStub writer = PkbWriterStub(pkb, writerAccessible);
//
//  ParentExtractor* extractor = new ParentExtractor(&writer);
//
//  extractor->visit(*simple);
//
//  REQUIRE(table->get(1) == unordered_set<int>({2}));
//  REQUIRE(reverseTable->get(2) == unordered_set<int>({1}));
//  REQUIRE(table->get(1) == unordered_set<int>({3}));
//  REQUIRE(reverseTable->get(3) == unordered_set<int>({1}));
//  REQUIRE(table->get(1) == unordered_set<int>({4}));
//  REQUIRE(reverseTable->get(4) == unordered_set<int>({1}));
//}
//
// TEST_CASE("ParentExtractor simple While") {}