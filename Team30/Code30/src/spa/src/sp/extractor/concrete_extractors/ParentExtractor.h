#pragma once

#include "sp/extractor/AbstractExtractor.h"
#include "sp/SPTypes.h"

class ParentExtractor : public AbstractExtractor {
 public:
  explicit ParentExtractor(PkbWriter *pkbWriter);
  void visitIf(const IfNode *node) override;
  void visitWhile(const WhileNode *node) override;

 private:
  void addParentOnList(LineNumber parentLine, ASTNodeRefList *childList);
  void addParentRelation(LineNumber x, LineNumber y);
  PkbWriter *pkbWriter;
};
