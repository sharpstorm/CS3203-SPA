#include "CFGExtractor.h"

#include <vector>

#include "StatementNumberExtractor.h"

using std::vector;

CFGExtractor::CFGExtractor(PkbWriter* writer) : pkbWriter(writer) {
  CFGcache = make_shared<CFG>();
}

void CFGExtractor::visit(ProcedureNode* node) {
  CFGcache = make_shared<CFG>((node->getName()));
}

void CFGExtractor::leave(ProcedureNode* node) { setOfCFGs.push_back(CFGcache); }

void CFGExtractor::visit(StatementListNode* node) {
  if (node->getChildren().empty()) {
    return;
  }

  vector<ASTNodePtr> children = node->getChildren();
  StatementNumberExtractor statementNoExtractor;

  if (CFGcache->getNodeMap().empty()) {
    children[0]->accept(&statementNoExtractor);
    int startingLine = statementNoExtractor.getStatementNumber() - 1;
    CFGcache->changeStartingLine(startingLine);
  }

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

void CFGExtractor::visit(IfNode* node) {
  vector<ASTNodePtr> children = node->getChildren();
  vector<ASTNodePtr> ifLst = children[1]->getChildren();
  vector<ASTNodePtr> elseLst = children[2]->getChildren();

  addCFGOnIfNodeList(node->getLineNumber(), &ifLst);
  addCFGOnIfNodeList(node->getLineNumber(), &elseLst);
}

void CFGExtractor::visit(WhileNode* node) {
  vector<ASTNodePtr> children = node->getChildren();
  vector<ASTNodePtr> stmtList = children[1]->getChildren();

  addCFGOnWhileNodeList(node->getLineNumber(), &stmtList);
}

vector<CFGPtr> CFGExtractor::getSetOfCFGs() { return setOfCFGs; }

void CFGExtractor::addCFGOnIfNodeList(int conditionalLine,
                                      vector<ASTNodePtr>* childList) {
  size_t childListSize = childList->size();

  if (childListSize == 0) {
    return;
  }

  StatementNumberExtractor statementNoExtractor;
  childList->at(0)->accept(
      &statementNoExtractor);  // to connect if statement to beginning of list
  CFGExtractor::addCFGRelation(conditionalLine,
                               statementNoExtractor.getStatementNumber());
}

void CFGExtractor::addCFGOnWhileNodeList(int conditionalLine,
                                         vector<ASTNodePtr>* childList) {
  size_t childListSize = childList->size();

  if (childListSize == 0) {
    return;
  }

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

void CFGExtractor::addCFGRelation(int x, int y) { CFGcache->addNode(x, y); }

void CFGExtractor::addCFGToPKB() {
  // pkbWriter->addCFGs(setOfCFGs);
  //
  // pkbWriter will need an empty vector of CFGs, which will be periodically
  // pushed new CFGs when leave(ProcedureNode) is called
}
