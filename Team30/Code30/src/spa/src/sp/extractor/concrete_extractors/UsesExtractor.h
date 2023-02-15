#pragma once

#include <string>
#include <memory>
#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::string;

class UsesExtractor : public AbstractExtractor {
 public:
  explicit UsesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(PrintNode node);
  void visit(WhileNode node);
  void visit(IfNode node);
  void leave(WhileNode node);
  void leave(IfNode node);

 private:
  void addUsesRelation(int x, string var);
  void processNode(int lineNumber, shared_ptr<ASTNode> expr);
  void recurseExpr(vector<string>* v, shared_ptr<ASTNode> node);
  bool arrayContains(vector<string> *v, string x);
  vector<int> statementStartStack;
  PkbWriter *pkbWriter;
};
