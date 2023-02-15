#pragma once

#include <string>
#include "sp/extractor/AbstractExtractor.h"

using std::string;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(ReadNode node);
  void visit(PrintNode node);
  void visit(WhileNode node);
  void visit(IfNode node);


 private:
  void addModifiesRelation(int x, string var);
  void checkStack(int lineNumber);
  vector<int> statementStartStack;
  vector<int> statementEndStack;
  PkbWriter *pkbWriter;
};
