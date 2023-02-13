#pragma once

#include <string>
#include "sp/extractor/AbstractExtractor.h"

using std::string;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(ReadNode node);


 private:
  void addModifiesRelation(int x, string var);
  PkbWriter *pkbWriter;
};
