#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

class UsesExtractor : public AbstractExtractor {
 public:
  explicit UsesExtractor(PkbWriter *pkbWriter);
  void visit(AssignNode node);
  void visit(PrintNode node);
  void visit(WhileNode node);
  void visit(IfNode node);

 private:
  void recurseExpr(vector<string>* v,
                   shared_ptr<ASTNode> node);
  bool arrayContains(vector<string> *v, string x);
  void addUsesRelation(int x, string var);
  void processNode(int lineNumber,
                   shared_ptr<ASTNode> expr);
  PkbWriter *pkbWriter;
};
