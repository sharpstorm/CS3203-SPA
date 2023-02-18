#include "ParentExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ParentExtractor::visit(IfNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> ifLst = children[1]->getChildren();
  vector<shared_ptr<ASTNode>> elseLst = children[2]->getChildren();
  for (int i = 0; i < ifLst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(ifLst[i])->lineNumber);
  }

  for (int i = 0; i < elseLst.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(elseLst[i])->lineNumber);
  }
}

void ParentExtractor::visit(WhileNode node) {
  vector<shared_ptr<ASTNode>> children = node.getChildren();
  vector<shared_ptr<ASTNode>> stmtList = children[1]->getChildren();
  for (int i = 0; i < stmtList.size(); i++) {
    ParentExtractor::addParentRelation(
        node.lineNumber,
        std::dynamic_pointer_cast<StatementASTNode>(stmtList[i])->lineNumber);
  }
}

void ParentExtractor::addParentRelation(int x, int y) {
  pkbWriter->addParent(x, y);
}
