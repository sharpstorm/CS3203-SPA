#include "CFGExtractor.h"

#include <memory>

#include "StatementNumberExtractor.h"

using std::make_shared;

CFGExtractor::CFGExtractor(PkbWriter* writer) :
    pkbWriter(writer),
    cfgCache(nullptr) {}

void CFGExtractor::visitProcedure(const ProcedureNode* node) {
  StatementNumberExtractor statementNoExtractor;

  node->getChildStatement(0)->accept(&statementNoExtractor);
  LineNumber startingLineIndex = statementNoExtractor.getStatementNumber();

  procedureNameCache = node->getName();
  cfgCache = make_shared<CFG>(startingLineIndex);
  clearableLastLines.push_back(startingLineIndex);
  resetPoint.push(startingLineIndex);
  resetCounters.push(0);
  cachedLastLines.push(vector<LineNumber>{});
}

void CFGExtractor::leaveProcedure(const ProcedureNode* node) {
  while (!clearableLastLines.empty()) {
    addCFGRelation(clearableLastLines.back(), CFG_END_NODE);
    clearableLastLines.pop_back();
  }

  addCFGToPKB(cfgCache);
  cfgCache = nullptr;
}

void CFGExtractor::leaveStmtList(const StatementListNode *node) {
  int curCount = resetCounters.top();
  resetCounters.pop();

  if (curCount > 0) {
    flushStack(&clearableLastLines, &cachedLastLines.top());
    clearableLastLines.push_back(resetPoint.top());
    resetCounters.push(curCount - 1);
    return;
  }

  flushStack(&cachedLastLines.top(), &clearableLastLines);
  resetPoint.pop();
  cachedLastLines.pop();
}

void CFGExtractor::visitIf(const IfNode* node) {
  advanceStatement(node);
  resetPoint.push(node->getLineNumber());
  resetCounters.push(1);
  cachedLastLines.push(vector<LineNumber>{});
}

void CFGExtractor::visitWhile(const WhileNode* node) {
  advanceStatement(node);
  resetPoint.push(node->getLineNumber());
  resetCounters.push(0);
  cachedLastLines.push(vector<LineNumber>{});
}

void CFGExtractor::leaveWhile(const WhileNode* node) {
  advanceStatement(node);
}

void CFGExtractor::visitAssign(const AssignNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitRead(const ReadNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitPrint(const PrintNode *node) {
  advanceStatement(node);
}

void CFGExtractor::visitCall(const CallNode *node) {
  advanceStatement(node);
}

void CFGExtractor::advanceStatement(const StatementASTNode* node) {
  LineNumber nodeNum = node->getLineNumber();
  while (!clearableLastLines.empty()) {
    addCFGRelation(clearableLastLines.back(), nodeNum);
    clearableLastLines.pop_back();
  }
  clearableLastLines.push_back(nodeNum);
}

void CFGExtractor::addCFGRelation(LineNumber from, LineNumber to) {
  if (from == to) {
    return;
  }
  cfgCache->addLink(cfgCache->toCFGNode(from),
                    cfgCache->toCFGNode(to));
}

void CFGExtractor::addCFGToPKB(CFGSPtr cfg) {
  pkbWriter->addCFGs(procedureNameCache, cfg);
}

void CFGExtractor::flushStack(vector<LineNumber> *source,
                              vector<LineNumber> *target) {
  for (const LineNumber item : *source) {
    target->push_back(item);
  }
  source->clear();
  VectorStack<LineNumber> temp;
}
