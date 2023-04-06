#pragma once

#include <vector>

#include "common/cfg/CFG.h"
#include "sp/extractor/AbstractExtractor.h"
#include "common/data_structs/VectorStack.h"
#include "sp/SPTypes.h"

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter *pkbWriter);
  void visitProcedure(const ProcedureNode *node) override;
  void leaveProcedure(const ProcedureNode *node) override;
  void leaveStmtList(const StatementListNode *node) override;

  void visitAssign(const AssignNode *node) override;
  void visitRead(const ReadNode *node) override;
  void visitPrint(const PrintNode *node) override;
  void visitCall(const CallNode *node) override;

  void visitIf(const IfNode *node) override;
  void visitWhile(const WhileNode *node) override;
  void leaveWhile(const WhileNode *node) override;

 private:
  void advanceStatement(const StatementASTNode *node);
  void addCFGRelation(LineNumber from, LineNumber to);

  void flushStack(vector<LineNumber> *source,
                  vector<LineNumber> *target);

  PkbWriter *pkbWriter;
  CFGSPtr cfgCache;
  ProcedureName procedureNameCache;
  VectorStack<LineNumber> resetPoint;
  VectorStack<LineNumber> resetCounters;

  VectorStack<vector<LineNumber>> cachedLastLines;
  vector<LineNumber> clearableLastLines;

 protected:
  virtual void addCFGToPKB(CFGSPtr cfg);
};
