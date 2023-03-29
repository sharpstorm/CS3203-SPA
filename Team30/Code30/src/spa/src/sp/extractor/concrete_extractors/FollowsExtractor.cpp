#include "FollowsExtractor.h"

#include <vector>
#include "StatementNumberExtractor.h"

using std::vector;

FollowsExtractor::FollowsExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void FollowsExtractor::visitStmtList(StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }
  vector<ASTNode*> children = node->getChildren();
  StatementNumberExtractor statementNoExtractor;

  for (int i = 0; i < children.size() - 1; i++) {
    children[i]->accept(&statementNoExtractor);
    int lineNoLeft = statementNoExtractor.getStatementNumber();
    children[i + 1]->accept(&statementNoExtractor);
    int lineNoRight = statementNoExtractor.getStatementNumber();
    addFollowsRelation(lineNoLeft, lineNoRight);
  }
}

void FollowsExtractor::addFollowsRelation(LineNumber x, LineNumber y) {
  pkbWriter->addFollows(x, y);
}
