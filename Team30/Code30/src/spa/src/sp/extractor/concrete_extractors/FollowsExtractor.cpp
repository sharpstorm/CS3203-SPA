#include "FollowsExtractor.h"

#include <vector>
#include <memory>
#include "StatementNumberExtractor.h"

using std::vector, std::make_shared;

FollowsExtractor::FollowsExtractor(PkbWriter* writer) : pkbWriter(writer) {
}

void FollowsExtractor::visitStmtList(StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }
  stmtLstCache.push(make_shared<LineNumbers>());
}

void FollowsExtractor::visitRead(ReadNode* node) {
  processStatement(node);
}

void FollowsExtractor::visitPrint(PrintNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitWhile(WhileNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitIf(IfNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitAssign(AssignNode *node) {
  processStatement(node);
}

void FollowsExtractor::visitCall(CallNode *node) {
  processStatement(node);
}

void FollowsExtractor::leaveStmtList(StatementListNode *node) {
  stmtLstCache.pop();
}

void FollowsExtractor::processStatement(StatementASTNode *node) {
  StatementNumberExtractor statementNoExtractor;
  node->accept(&statementNoExtractor);
  for (LineNumber line : *stmtLstCache.top()) {
    addFollowsRelation(line, statementNoExtractor.getStatementNumber());
  }
  stmtLstCache.top()->push_back(statementNoExtractor.getStatementNumber());
}

void FollowsExtractor::addFollowsRelation(LineNumber x, LineNumber y) {
  pkbWriter->addFollows(x, y);
}
