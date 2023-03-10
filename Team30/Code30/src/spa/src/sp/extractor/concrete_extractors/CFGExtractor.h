#pragma once

#include <memory>
#include <vector>

#include "../../ast/CFG.h"
#include "sp/extractor/AbstractExtractor.h"

typedef shared_ptr<CFG> CFGPtr;

using std::make_shared;

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter* pkbWriter);
  void visitProcedure(ProcedureNode* node) override;
  void leaveProcedure(ProcedureNode* node) override;
  void visitStmtList(StatementListNode* node) override;
  void visitIf(IfNode* node) override;
  void visitWhile(WhileNode* node) override;

  vector<CFGPtr> getSetOfCFGs();

 private:
  void addCFGOnIfNodeList(int conditionalLine, vector<ASTNodePtr>* childList);
  void addCFGOnWhileNodeList(int conditionalLine,
                             vector<ASTNodePtr>* childList);
  void addCFGRelation(int x, int y);
  void addCFGToPKB();
  PkbWriter* pkbWriter;
  CFGPtr CFGcache;
  vector<CFGPtr> setOfCFGs;
};
