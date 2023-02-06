#include "FollowsExtractor.h"

void FollowsExtractor::visit(StatementListNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  for (int i = 0; i < children.size() - 1; i++) {
    addFollowsRelation(
        std::dynamic_pointer_cast<StatementASTNode>(children[i])->lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(children[i + 1])
            ->lineNumber);
  }
}

void FollowsExtractor::addFollowsRelation(int x, int y) {
  FollowsExtractor::writer.addFollows(x, y);
}
