#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include <stack>
#include "sp/extractor/AbstractExtractor.h"

using std::string, std::unordered_set, std::vector, std::stack;

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
  void visitStmtList(StatementListNode* node) override;
  void visitVariable(VariableASTNode* node) override;
  void leaveAssign(AssignNode* node) override;

 private:
  void addUsesRelation(const LineNumber &x, const string &var);
  void processNode(const LineNumber &lineNumber,
                   const unordered_set<string> &v);
  void updateUses(const unordered_set<string> &v);
  void updateUses(const string &v);
  void leave();
  void visit(bool addToPkb, bool isDisabled, LineNumber lineNumber);

  vector<LineNumber> statementStartStack;
  vector<bool> addToPKB;
  vector<bool> isDisabledFromContainer;
  bool oneShot;
  unordered_set<string> readVars;
  PkbWriter *pkbWriter;
  string procName;
};
