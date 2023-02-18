#include "FollowsExtractor.h"
#include "StatementNumberExtractor.h"

FollowsExtractor::FollowsExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void FollowsExtractor::visit(StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }
  vector<shared_ptr<ASTNode>> children = node->getChildren();
  StatementNumberExtractor statementNoExtractor;

  for (int i = 0; i < children.size() - 1; i++) {
    children[i]->accept(&statementNoExtractor);
    int lineNoLeft = statementNoExtractor.getStatementNumber();
    children[i + 1]->accept(&statementNoExtractor);
    int lineNoRight = statementNoExtractor.getStatementNumber();
    addFollowsRelation(lineNoLeft, lineNoRight);
  }
}

void FollowsExtractor::addFollowsRelation(int x, int y) {
  pkbWriter->addFollows(x, y);
}
