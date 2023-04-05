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
  void addParentOnList(LineNumber parentLine, vector<ASTNode*>* childList);
  void addParentRelation(LineNumber x, LineNumber y);
  PkbWriter* pkbWriter;
};
