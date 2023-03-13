#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <string>

#include "common/cfg/CFG.h"
#include "sp/extractor/AbstractExtractor.h"

using std::make_shared, std::stack, std::string;

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter* pkbWriter);
  void visitProcedure(ProcedureNode* node) override;
  void leaveProcedure(ProcedureNode* node) override;
  void leaveStmtList(StatementListNode* node) override;

  void visitAssign(AssignNode* node) override;
  void visitRead(ReadNode* node) override;
  void visitPrint(PrintNode* node) override;
  void visitCall(CallNode* node) override;

  void visitIf(IfNode* node) override;
  void visitWhile(WhileNode* node) override;
  void leaveWhile(WhileNode* node) override;

 private:
  void advanceStatement(StatementASTNode* node);
  void addCFGRelation(int from, int to);

  void flushStack(stack<int>* source, stack<int>* target);

  PkbWriter* pkbWriter;
  CFGSPtr cfgCache;
  string procedureNameCache;
  stack<int> resetPoint;
  stack<int> resetCounters;

  stack<stack<int>> cachedLastLines;
  stack<int> clearableLastLines;

 protected:
  virtual void addCFGToPKB(CFGSPtr cfg);
};
