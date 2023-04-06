#pragma once

#include <vector>
#include "sp/extractor/AbstractExtractor.h"
#include "sp/SPTypes.h"

using std::vector;

class UsesExtractor : public AbstractExtractor {
 public:
  explicit UsesExtractor(PkbWriter *pkbWriter);
  void visitAssign(const AssignNode* node) override;
  void visitPrint(const PrintNode* node) override;
  void visitWhile(const WhileNode* node) override;
  void visitIf(const IfNode* node) override;
  void leaveWhile(const WhileNode* node) override;
  void leaveIf(const IfNode* node) override;
  void visitProcedure(const ProcedureNode* node) override;
  void visitStmtList(const StatementListNode* node) override;
  void visitVariable(const VariableASTNode* node) override;
  void leaveAssign(const AssignNode* node) override;

 private:
  void addUsesRelation(const LineNumber &x, const VariableName &var);
  void processNode(const LineNumber &lineNumber,
                   const VariableNameSet &v);
  void updateUses(const VariableNameSet &v);
  void updateUses(const VariableName &v);
  void leave();
  void visit(bool addToPkb, bool isDisabled, LineNumber lineNumber);

  vector<LineNumber> statementStartStack;
  vector<bool> addToPKB;
  vector<bool> isDisabledFromContainer;
  bool oneShot;
  VariableNameSet readVars;
  PkbWriter *pkbWriter;
  ProcedureName procName;
};
