#include "ParentExtractor.h"
#include "StatementNumberExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ParentExtractor::visit(IfNode* node) {
  vector<shared_ptr<ASTNode>> children = node->getChildren();
  vector<shared_ptr<ASTNode>> ifLst = children[1]->getChildren();
  vector<shared_ptr<ASTNode>> elseLst = children[2]->getChildren();

  StatementNumberExtractor statementNoExtractor;
  for (int i = 0; i < ifLst.size(); i++) {
    ifLst[i]->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        node->getLineNumber(),
        statementNoExtractor.getStatementNumber());
  }

  for (int i = 0; i < elseLst.size(); i++) {
    elseLst[i]->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        node->getLineNumber(),
        statementNoExtractor.getStatementNumber());
  }
}

void ParentExtractor::visit(WhileNode* node) {
  vector<shared_ptr<ASTNode>> children = node->getChildren();
  vector<shared_ptr<ASTNode>> stmtList = children[1]->getChildren();

  StatementNumberExtractor statementNoExtractor;
  for (int i = 0; i < stmtList.size(); i++) {
    stmtList[i]->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        node->getLineNumber(),
        statementNoExtractor.getStatementNumber());
  }
}

void ParentExtractor::addParentRelation(int x, int y) {
  pkbWriter->addParent(x, y);
}
