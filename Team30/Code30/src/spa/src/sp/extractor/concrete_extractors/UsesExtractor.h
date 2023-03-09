#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include "sp/extractor/AbstractExtractor.h"

using std::string, std::unordered_set, std::vector;

class UsesExtractor : public AbstractExtractor {
 public:
  explicit UsesExtractor(PkbWriter *pkbWriter);
  void visitAssign(AssignNode* node) override;
  void visitPrint(PrintNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
  void leaveWhile(WhileNode* node) override;
  void leaveIf(IfNode* node) override;
  void visitProcedure(ProcedureNode* node) override;
 private:
  void addUsesRelation(int x, string var);
  void processNode(int lineNumber, unordered_set<string>* v);
  void recurseExpr(unordered_set<string>* v, shared_ptr<ASTNode> node);
  bool setContains(unordered_set<string> *v, const string &x);
  void updateUses(shared_ptr<ASTNode> expr, int lineNumber);
  vector<int> statementStartStack;
  PkbWriter *pkbWriter;
  string procName;
};
