#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class StatementListExtractor : public AbstractExtractor {
 public:
  explicit StatementListExtractor(PkbWriter* pkbWriter);
  void visit(StatementListNode node);

 private:
  void addStatementListRange(int x, int y);
  PkbWriter* pkbWriter;
};
