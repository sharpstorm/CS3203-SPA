#pragma once

#include "../AbstractExtractor.h"
#include "sp/SPTypes.h"

class StatementNumberExtractor : public AbstractExtractor {
 public:
  StatementNumberExtractor(): statementNumber(0) {}
  void visitPrint(const PrintNode* node) override;
  void visitAssign(const AssignNode* node) override;
  void visitWhile(const WhileNode* node) override;
  void visitIf(const IfNode* node) override;
  void visitRead(const ReadNode* node) override;
  void visitCall(const CallNode* node) override;
  const int getStatementNumber();

 private:
  void updateLineNumber(const StatementASTNode* node);
  LineNumber statementNumber;
};
