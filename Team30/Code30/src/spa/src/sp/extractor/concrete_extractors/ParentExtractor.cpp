#include "ParentExtractor.h"
#include "StatementNumberExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void ParentExtractor::visitIf(IfNode* node) {
  vector<ASTNodePtr> children = node->getChildren();
  vector<ASTNodePtr> ifLst = children[1]->getChildren();
  vector<ASTNodePtr> elseLst = children[2]->getChildren();

  addParentOnList(node->getLineNumber(), &ifLst);
  addParentOnList(node->getLineNumber(), &elseLst);
}

void ParentExtractor::visitWhile(WhileNode* node) {
  vector<ASTNodePtr> children = node->getChildren();
  vector<ASTNodePtr> stmtList = children[1]->getChildren();

  addParentOnList(node->getLineNumber(), &stmtList);
}

void ParentExtractor::addParentOnList(int parentLine,
                                      vector<ASTNodePtr> *childList) {
  StatementNumberExtractor statementNoExtractor;
  for (int i = 0; i < childList->size(); i++) {
    childList->at(i)->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        parentLine,
        statementNoExtractor.getStatementNumber());
  }
}

void ParentExtractor::addParentRelation(int x, int y) {
  pkbWriter->addParent(x, y);
}
