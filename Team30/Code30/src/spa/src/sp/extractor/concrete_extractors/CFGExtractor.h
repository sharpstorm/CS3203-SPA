#pragma once

#include <memory>
#include <vector>

#include "common/cfg/CFG.h"
#include "sp/extractor/AbstractExtractor.h"

using std::make_shared;

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter* pkbWriter);
  void visitProcedure(ProcedureNode* node) override;
  void leaveProcedure(ProcedureNode* node) override;
  void visitStmtList(StatementListNode* node) override;
  void visitIf(IfNode* node) override;
  void visitWhile(WhileNode* node) override;

 private:
  void addCFGOnIfNodeList(int conditionalLine,
                          vector<ASTNode*>* childList);
  void addCFGOnWhileNodeList(int conditionalLine,
                             vector<ASTNode*>* childList);
  void addCFGRelation(int x, int y);
  void addCFGToPKB();

  PkbWriter* pkbWriter;

 protected:
  CFGSPtr cfgCache;
};
