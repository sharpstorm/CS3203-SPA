#pragma once

#include "../AbstractExtractor.h"

class StatementNumberExtractor : public AbstractExtractor {
 public:
  StatementNumberExtractor(): statementNumber(0) {}
  void visitPrint(PrintNode* node) override;
  void visitAssign(AssignNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
  void visitRead(ReadNode* node) override;
  void visitCall(CallNode* node) override;
  const int getStatementNumber();

 private:
  LineNumber statementNumber;
};
