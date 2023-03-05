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
  void visit(AssignNode* node);
  void visit(PrintNode* node);
  void visit(WhileNode* node);
  void visit(IfNode* node);
  void leave(WhileNode* node);
  void leave(IfNode* node);
  void visit(ProcedureNode* node);
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
