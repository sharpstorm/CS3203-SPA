#pragma once

#include <vector>

#include "../../ast/CFG.h"
#include "sp/extractor/AbstractExtractor.h"

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter* pkbWriter);
  void visit(ProcedureNode* node);
  void leave(ProcedureNode* node);
  void visit(StatementListNode* node);
  void visit(IfNode* node);
  void visit(WhileNode* node);

  vector<CFG> getSetOfCFGs();

 private:
  void addCFGOnIfNodeList(int conditionalLine, vector<ASTNodePtr>* childList);
  void addCFGOnWhileNodeList(int conditionalLine,
                             vector<ASTNodePtr>* childList);
  void addCFGRelation(int x, int y);
  void addCFGToPKB();
  PkbWriter* pkbWriter;
  CFG* CFGcache;
  vector<CFG> setOfCFGs;
  int index;
};
