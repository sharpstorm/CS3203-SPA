#include "ParentExtractor.h"
#include "StatementNumberExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter *writer) : pkbWriter(writer) {
}

void ParentExtractor::visitIf(const IfNode *node) {
  ASTNodeRefList children = node->getChildren();
  ASTNode *ifLst = children[1];
  ASTNode *elseLst = children[2];

  addParentOnList(node->getLineNumber(), ifLst);
  addParentOnList(node->getLineNumber(), elseLst);
}

void ParentExtractor::visitWhile(const WhileNode *node) {
  ASTNodeRefList children = node->getChildren();
  ASTNode *stmtList = children[1];

  addParentOnList(node->getLineNumber(), stmtList);
}

void ParentExtractor::addParentOnList(LineNumber parentLine,
                                      ASTNode *stmtList) {
  StatementNumberExtractor statementNoExtractor;
  for (const ASTNode *node : stmtList->getChildren()) {
    node->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        parentLine,
        statementNoExtractor.getStatementNumber());
  }
}

void ParentExtractor::addParentRelation(LineNumber x, LineNumber y) {
  pkbWriter->addParent(x, y);
}
