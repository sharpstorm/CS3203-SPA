#include "ParentExtractor.h"

void ParentExtractor::visit(IfNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> stmtlst = children[1]->getChildren();
  for (int i = 0; i < stmtlst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(children[i])->lineNumber);
  }
}

void ParentExtractor::visit(WhileNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> stmtlst = children[1]->getChildren();
  for (int i = 0; i < stmtlst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(children[i])->lineNumber);
  }
}

void ParentExtractor::addParentRelation(int x, int y) {
  ParentExtractor::writer.addParent(x, y);
}