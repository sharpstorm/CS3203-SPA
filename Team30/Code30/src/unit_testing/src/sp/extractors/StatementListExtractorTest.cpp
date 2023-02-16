#include "../../../../../lib/catch.hpp"
#include "../../../../spa/src/common/ASTNode/StatementListNode.h"
#include "../../../../spa/src/common/ASTNode/statement/AssignNode.h"
#include "../../../../spa/src/common/ASTNode/statement/PrintNode.h"
#include "../../../../spa/src/pkb/storage/PKB.h"
#include "../../../../spa/src/pkb/writers/PkbWriter.h"
#include "../../../../spa/src/sp/extractor/concrete_extractors/StatementListExtractor.h"
class PkbWriterStubforStmt : public PkbWriter {
 public:
  int start;
  int end;

  PkbWriterStubforStmt(PKB* pkb) : PkbWriter(pkb) {}

  void addContainerStmt(int x, int y) final {
    start = x;
    end = y;
  }

  int checkFirstAndLast(int x, int y) { return (start == x) && (end == y); }
};

TEST_CASE("StatementListExtractor single node") {
  shared_ptr<StatementListNode> stmtList =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> firstNode =
      shared_ptr<StatementASTNode>(new PrintNode());

  firstNode->lineNumber = 5;

  stmtList->addChild(firstNode);

  PKB* pkb = new PKB();

  PkbWriterStubforStmt* writer = new PkbWriterStubforStmt(pkb);

  StatementListExtractor* extractor = new StatementListExtractor(writer);

  extractor->visit(*stmtList);

  REQUIRE(writer->checkFirstAndLast(5, 5));
}

TEST_CASE("StatementListExtractor multiple nodes") {
  shared_ptr<StatementListNode> stmtList =
      shared_ptr<StatementListNode>(new StatementListNode());

  shared_ptr<StatementASTNode> firstNode =
      shared_ptr<StatementASTNode>(new PrintNode());

  shared_ptr<StatementASTNode> secondNode =
      shared_ptr<StatementASTNode>(new AssignNode());

  shared_ptr<StatementASTNode> thirdNode =
      shared_ptr<StatementASTNode>(new PrintNode());

  firstNode->lineNumber = 5;
  secondNode->lineNumber = 6;
  thirdNode->lineNumber = 7;

  stmtList->addChild(firstNode);
  stmtList->addChild(secondNode);
  stmtList->addChild(thirdNode);

  PKB* pkb = new PKB();

  PkbWriterStubforStmt* writer = new PkbWriterStubforStmt(pkb);

  StatementListExtractor* extractor = new StatementListExtractor(writer);

  extractor->visit(*stmtList);

  REQUIRE(writer->checkFirstAndLast(5, 7));
}