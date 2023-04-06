#pragma once

#include <string>
#include <vector>
#include "sp/extractor/AbstractExtractor.h"

using std::string, std::vector;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visitAssign(AssignNode* node) override;
  void visitRead(ReadNode* node) override;
  void visitWhile(WhileNode* node) override;
  void leaveWhile(WhileNode* node) override;
  void visitVariable(VariableASTNode* node) override;
  void visitIf(IfNode* node) override;
  void leaveIf(IfNode* node) override;
  void visitProcedure(ProcedureNode* node) override;

 private:
  void addNodeModifies(const LineNumber &lineNo, const string &var);
  void addModifiesRelation(LineNumber x, string var);
  vector<LineNumber> statementStartStack;
  PkbWriter *pkbWriter;
  string currentProcName;
  int curStatement;
};
