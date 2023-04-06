#pragma once

#include <vector>
#include "sp/extractor/AbstractExtractor.h"
#include "sp/SPTypes.h"

using std::vector;

class ModifiesExtractor : public AbstractExtractor {
 public:
  explicit ModifiesExtractor(PkbWriter *pkbWriter);
  void visitAssign(const AssignNode* node) override;
  void visitRead(const ReadNode* node) override;
  void visitWhile(const WhileNode* node) override;
  void leaveWhile(const WhileNode* node) override;
  void visitVariable(const VariableASTNode* node) override;
  void visitIf(const IfNode* node) override;
  void leaveIf(const IfNode* node) override;
  void visitProcedure(const ProcedureNode* node) override;

 private:
  void addNodeModifies(const LineNumber &lineNo, const VariableName &var);
  void addModifiesRelation(LineNumber x, const VariableName &var);
  vector<LineNumber> statementStartStack;
  PkbWriter *pkbWriter;
  ProcedureName currentProcName;
  LineNumber curStatement;

  static const LineNumber NO_LINE = -1;
};
