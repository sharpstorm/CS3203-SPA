#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class ParentExtractor : public AbstractExtractor {
 public:
  explicit ParentExtractor(PkbWriter* pkbWriter);
  virtual void visit(IfNode node);
  virtual void visit(WhileNode node);

 private:
  void addParentRelation(int x, int y);
  PkbWriter* pkbWriter;
};
