#include "ParentExtractor.h"
#include "StatementNumberExtractor.h"

ParentExtractor::ParentExtractor(PkbWriter *writer) : pkbWriter(writer) {
}

void ParentExtractor::visitIf(const IfNode *node) {
  ASTNodeRefList children = node->getChildren();
  ASTNodeRefList ifLst = children[1]->getChildren();
  ASTNodeRefList elseLst = children[2]->getChildren();

  addParentOnList(node->getLineNumber(), &ifLst);
  addParentOnList(node->getLineNumber(), &elseLst);
}

void ParentExtractor::visitWhile(const WhileNode *node) {
  ASTNodeRefList children = node->getChildren();
  ASTNodeRefList stmtList = children[1]->getChildren();

  addParentOnList(node->getLineNumber(), &stmtList);
}

void ParentExtractor::addParentOnList(LineNumber parentLine,
                                      ASTNodeRefList *childList) {
  StatementNumberExtractor statementNoExtractor;
  for (const ASTNode *node : *childList) {
    node->accept(&statementNoExtractor);
    ParentExtractor::addParentRelation(
        parentLine,
        statementNoExtractor.getStatementNumber());
  }
}

void ParentExtractor::addParentRelation(LineNumber x, LineNumber y) {
  pkbWriter->addParent(x, y);
}
