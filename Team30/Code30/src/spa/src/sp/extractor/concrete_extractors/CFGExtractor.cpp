#include "CFGExtractor.h"

#include <vector>

#include "StatementNumberExtractor.h"

using std::vector;

CFGExtractor::CFGExtractor(PkbWriter* writer) :
    pkbWriter(writer),
    cfgCache(nullptr) {}

void CFGExtractor::visitProcedure(ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;

  node->getChildStatement(0)->accept(&statementNoExtractor);
  int startingLineIndex = statementNoExtractor.getStatementNumber();

  cfgCache = make_shared<CFG>(node->getName(), startingLineIndex);
  clearableLastLines.push(startingLineIndex);
  resetPoint.push(startingLineIndex);
  resetCounters.push(0);
  cachedLastLines.push(stack<int>{});
}

void CFGExtractor::leaveProcedure(ProcedureNode* node) {
  while (!clearableLastLines.empty()) {
    addCFGRelation(clearableLastLines.top(), CFG_END_NODE);
    clearableLastLines.pop();
  }

  addCFGToPKB(cfgCache);
  cfgCache = nullptr;
}

void CFGExtractor::leaveStmtList(StatementListNode *node) {
  int curCount = resetCounters.top();
  resetCounters.pop();

  if (curCount > 0) {
    flushStack(&clearableLastLines, &cachedLastLines.top());
    clearableLastLines.push(resetPoint.top());
    resetCounters.push(curCount - 1);
    return;
  }

  flushStack(&cachedLastLines.top(), &clearableLastLines);
  resetPoint.pop();
  cachedLastLines.pop();
}

void CFGExtractor::visitIf(IfNode* node) {
  advanceStatement(node);
  resetPoint.push(node->getLineNumber());
  resetCounters.push(1);
  cachedLastLines.push(stack<int>{});
}

void CFGExtractor::visitWhile(WhileNode* node) {
  advanceStatement(node);
  resetPoint.push(node->getLineNumber());
  resetCounters.push(0);
  cachedLastLines.push(stack<int>{});
}

void CFGExtractor::leaveWhile(WhileNode* node) {
  stack<int> temp;
  int value;
  while (!clearableLastLines.empty()) {
    value = clearableLastLines.top();
    addCFGRelation(value, node->getLineNumber());
    temp.push(value);
    clearableLastLines.pop();
  }

  while (!temp.empty()) {
    clearableLastLines.push(temp.top());
    temp.pop();
  }
}

void CFGExtractor::visitAssign(AssignNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitRead(ReadNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitPrint(PrintNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitCall(CallNode *node) {
  advanceStatement(node);
}

void CFGExtractor::advanceStatement(StatementASTNode* node) {
  int nodeNum = node->getLineNumber();
  while (!clearableLastLines.empty()) {
    addCFGRelation(clearableLastLines.top(), nodeNum);
    clearableLastLines.pop();
  }
  clearableLastLines.push(nodeNum);
}

void CFGExtractor::addCFGRelation(int from, int to) {
  if (from == to) {
    return;
  }
  cfgCache->addLink(from, to);
}

void CFGExtractor::addCFGToPKB(CFGSPtr cfg) {
  // pkbWriter->addCFGs(cfgCache);
  //
  // pkbWriter will need an empty vector of CFGs, which will be periodically
  // pushed new CFGs when leave(ProcedureNode) is called
}

void CFGExtractor::flushStack(stack<int> *source, stack<int> *target) {
  stack<int> temp;
  while (!source->empty()) {
    temp.push(source->top());
    source->pop();
  }

  while (!temp.empty()) {
    target->push(temp.top());
    temp.pop();
  }
}
