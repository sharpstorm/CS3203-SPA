#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void FollowsExtractor::visit(StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }
  vector<shared_ptr<ASTNode>> children = node->getChildren();
  for (int i = 0; i < children.size() - 1; i++) {
    addFollowsRelation(
        std::dynamic_pointer_cast<StatementASTNode>(children[i])->getLineNumber(),
        std::dynamic_pointer_cast<StatementASTNode>(children[i + 1])
            ->getLineNumber());
  }
}

void FollowsExtractor::addFollowsRelation(int x, int y) {
  pkbWriter->addFollows(x, y);
}
