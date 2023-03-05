#pragma once

#include <string>
#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::string, std::vector;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode* node);
  void visit(ReadNode* node);
  void visit(WhileNode* node);
  void leave(WhileNode* node);
  void visit(IfNode* node);
  void leave(IfNode* node);
  void visit(ProcedureNode* node);

 private:
  void addNodeModifies(StatementASTNode* node, const string &var);
  void addModifiesRelation(int x, string var);
  vector<int> statementStartStack;
  PkbWriter *pkbWriter;
  string currentProcName;
};
