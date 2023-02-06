#pragma once

#include "sp/extractor/AbstractExtractor.h"

class ParentExtractor : public AbstractExtractor {
 public:
  ParentExtractor(PkbWriter* pkbWriter);
  virtual void visit(IfNode node);
  virtual void visit(WhileNode node);

 private:
  void addParentRelation(int x, int y);
  PkbWriter* pkbWriter;
};
