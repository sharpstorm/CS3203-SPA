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
  void visit(ProcedureNode* node);
  void leave(ProcedureNode* node);
  void visit(StatementListNode* node);
  void visit(IfNode* node);
  void visit(WhileNode* node);

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
