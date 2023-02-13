#include "StatementListExtractor.h"

StatementListExtractor::StatementListExtractor(PkbWriter* writer) { pkbWriter = writer; }

void StatementListExtractor::visit(StatementListNode node) {
  int start = std::dynamic_pointer_cast<StatementASTNode>
      (node.getChildren()[0])->lineNumber;
  int end = std::dynamic_pointer_cast<StatementASTNode>
      (node.getChildren()[node.getChildren().size() - 1])->lineNumber;
  addStatementListRange(start, end);
}

void StatementListExtractor::addStatementListRange(int x, int y) {
  pkbWriter->addContainerStmt(x, y);
}
