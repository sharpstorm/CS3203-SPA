#include "ParentExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter* writer) { pkbWriter = writer; }

void ParentExtractor::visit(IfNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> stmtlst = children[1]->getChildren();
  for (int i = 0; i < stmtlst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(stmtlst[i])->lineNumber);
  }
}

void ParentExtractor::visit(WhileNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> ifStmtLst = children[1]->getChildren();
  vector<shared_ptr<ASTNode>> elseStmtLst = children[2]->getChildren();
  for (int i = 0; i < ifStmtLst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(ifStmtLst[i])->lineNumber);
  }

  for (int i = 0; i < elseStmtLst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(elseStmtLst[i])
            ->lineNumber);
  }
}

void ParentExtractor::addParentRelation(int x, int y) {
  pkbWriter->addParent(x, y);
}
