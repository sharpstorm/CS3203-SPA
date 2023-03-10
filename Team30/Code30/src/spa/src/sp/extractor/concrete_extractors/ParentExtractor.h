#pragma once

#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::vector;

class ParentExtractor : public AbstractExtractor {
 public:
  explicit ParentExtractor(PkbWriter* pkbWriter);
  void visitIf(IfNode* node) override;
  void visitWhile(WhileNode* node) override;

 private:
  void addParentOnList(int parentLine, vector<ASTNode*>* childList);
  void addParentRelation(int x, int y);
  PkbWriter* pkbWriter;
};
