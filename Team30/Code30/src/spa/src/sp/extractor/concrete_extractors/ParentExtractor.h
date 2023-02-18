#pragma once

#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::vector;

class ParentExtractor : public AbstractExtractor {
 public:
  explicit ParentExtractor(PkbWriter* pkbWriter);
  void visit(IfNode* node);
  void visit(WhileNode* node);

 private:
  void addParentOnList(int parentLine, vector<ASTNodePtr>* childList);
  void addParentRelation(int x, int y);
  PkbWriter* pkbWriter;
};
