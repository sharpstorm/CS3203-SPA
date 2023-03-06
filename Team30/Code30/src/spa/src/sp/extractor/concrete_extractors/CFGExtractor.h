#pragma once

#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class CFGExtractor : public AbstractExtractor {
 public:
  explicit CFGExtractor(PkbWriter* pkbWriter);
  void visit(StatementListNode* node);
  void visit(IfNode* node);
  void visit(WhileNode* node);

 private:
  void addCFGOnIfNodeList(int conditionalLine, vector<ASTNodePtr>* childList);
  void addCFGOnWhileNodeList(int conditionalLine,
                             vector<ASTNodePtr>* childList);
  void addCFGRelation(int x, int y);
  PkbWriter* pkbWriter;
};
