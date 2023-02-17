#pragma once

#include <string>
#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::string;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(ReadNode node);
  void visit(WhileNode node);
  void leave(WhileNode node);
  void visit(IfNode node);
  void leave(IfNode node);

 private:
  void addModifiesRelation(int x, string var);
  vector<int> statementStartStack;
  PkbWriter *pkbWriter;
};
