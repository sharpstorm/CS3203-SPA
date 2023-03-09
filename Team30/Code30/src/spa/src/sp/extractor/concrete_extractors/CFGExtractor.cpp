#include "CFGExtractor.h"

#include <vector>

#include "StatementNumberExtractor.h"

using std::vector;

CFGExtractor::CFGExtractor(PkbWriter* writer) :
    pkbWriter(writer),
    cfgCache(nullptr) {}

void CFGExtractor::visitProcedure(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;

  node->getChildren()[0]->getChildren()[0]->accept(&statementNoExtractor);
  int startingLineIndex = statementNoExtractor.getStatementNumber() - 1;

  cfgCache = make_shared<CFG>(node->getName(), startingLineIndex);
}

void CFGExtractor::leaveProcedure(ProcedureNode* node) {
  addCFGToPKB();
  cfgCache = nullptr;
}

void CFGExtractor::visitStmtList(StatementListNode* node) {
  if (node->getChildCount() == 0) {
    return;
  }

  StatementNumberExtractor statementNoExtractor;
  vector<ASTNode*> children = node->getChildren();

  for (int i = 0; i < children.size() - 1; i++) {
    children[i]->accept(&statementNoExtractor);
    int lineNoLeft = statementNoExtractor.getStatementNumber();
    children[i + 1]->accept(&statementNoExtractor);
    int lineNoRight = statementNoExtractor.getStatementNumber();
    addCFGRelation(lineNoLeft, lineNoRight);
  }

  children[children.size() - 1]->accept(&statementNoExtractor);
  int lastLineNo = statementNoExtractor.getStatementNumber();

  addCFGRelation(lastLineNo, -1);
}

void CFGExtractor::visitIf(IfNode* node) {
  vector<ASTNode*> children = node->getChildren();
  vector<ASTNode*> ifLst = children[1]->getChildren();
  vector<ASTNode*> elseLst = children[2]->getChildren();

  addCFGOnIfNodeList(node->getLineNumber(), &ifLst);
  addCFGOnIfNodeList(node->getLineNumber(), &elseLst);
}

void CFGExtractor::visitWhile(WhileNode* node) {
  vector<ASTNode*> children = node->getChildren();
  vector<ASTNode*> stmtList = children[1]->getChildren();

  addCFGOnWhileNodeList(node->getLineNumber(), &stmtList);
}

void CFGExtractor::addCFGOnIfNodeList(int conditionalLine,
                                      vector<ASTNode*>* childList) {
  if (childList->empty()) {
    return;
  }

  StatementNumberExtractor statementNoExtractor;
  childList->at(0)->accept(
      &statementNoExtractor);  // to connect if statement to beginning of list
  CFGExtractor::addCFGRelation(conditionalLine,
                               statementNoExtractor.getStatementNumber());
}

void CFGExtractor::addCFGOnWhileNodeList(int conditionalLine,
                                         vector<ASTNode*>* childList) {
  if (childList->empty()) {
    return;
  }

  size_t childListSize = childList->size();
  StatementNumberExtractor firstStatementNoExtractor, lastStatementNoExtractor;
  childList->at(0)->accept(
      &firstStatementNoExtractor);  // to connect while statement to beginning
  // of list
  childList->at(childListSize - 1)
      ->accept(&lastStatementNoExtractor);  // to connect end of list to while
  // statement
  CFGExtractor::addCFGRelation(conditionalLine,
                               firstStatementNoExtractor.getStatementNumber());

  CFGExtractor::addCFGRelation(lastStatementNoExtractor.getStatementNumber(),
                               conditionalLine);
}

void CFGExtractor::addCFGRelation(int x, int y) {
  cfgCache->addLink(x, y);
}

void CFGExtractor::addCFGToPKB() {
  // pkbWriter->addCFGs(cfgCache);
  //
  // pkbWriter will need an empty vector of CFGs, which will be periodically
  // pushed new CFGs when leave(ProcedureNode) is called
}
