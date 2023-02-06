#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter* pkbWriter);
  virtual void visit(PrintNode node);
  virtual void visit(AssignNode node);
  virtual void visit(WhileNode node);
  virtual void visit(IfNode node);
  virtual void visit(ReadNode node);
 private:
  PkbWriter* pkbWriter;
};
