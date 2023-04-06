#include "FollowsExtractor.h"

#include "StatementNumberExtractor.h"

FollowsExtractor::FollowsExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void FollowsExtractor::visitStmtList(const StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }
  stmtLstCache.push(LineNumbers());
}

void FollowsExtractor::visitRead(const ReadNode* node) {
  processStatement(node);
}

void FollowsExtractor::visitPrint(const PrintNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitWhile(const WhileNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitIf(const IfNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitAssign(const AssignNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitCall(const CallNode *node) {
  processStatement(node);
}

void FollowsExtractor::leaveStmtList(const StatementListNode *node) {
  stmtLstCache.pop();
}

void FollowsExtractor::processStatement(const StatementASTNode *node) {
  StatementNumberExtractor statementNoExtractor;
  node->accept(&statementNoExtractor);
  for (const LineNumber &line : stmtLstCache.top()) {
    addFollowsRelation(line, statementNoExtractor.getStatementNumber());
  }
  stmtLstCache.top().push_back(statementNoExtractor.getStatementNumber());
}

void FollowsExtractor::addFollowsRelation(LineNumber x, LineNumber y) {
  pkbWriter->addFollows(x, y);
}
