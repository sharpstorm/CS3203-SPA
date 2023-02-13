#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(ReadNode node);


 private:
  void addModifiesRelation(int x, string var);
  PkbWriter *pkbWriter;
};
