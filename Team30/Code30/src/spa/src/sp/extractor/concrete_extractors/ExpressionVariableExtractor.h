#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include "sp/extractor/AbstractExtractor.h"

using std::string, std::unordered_set, std::vector;

class ExpressionVariableExtractor : public AbstractExtractor {
 public:
  ExpressionVariableExtractor();
  void visitAssign(AssignNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
  unordered_set<string> getVariableSet();
 private:
  void recurseExpr(unordered_set<string>* v, shared_ptr<ASTNode> node);
  bool setContains(unordered_set<string> *v, const string &x);
  unordered_set<string> variableSetCache;
};
