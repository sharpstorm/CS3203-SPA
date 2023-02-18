#pragma once

#include "../AbstractExtractor.h"

class StatementNumberExtractor : public AbstractExtractor {
 public:
  StatementNumberExtractor(): statementNumber(0) {}
  void visit(PrintNode* node);
  void visit(AssignNode* node);
  void visit(WhileNode* node);
  void visit(IfNode* node);
  void visit(ReadNode* node);
  void visit(CallNode* node);
  int getStatementNumber();

 private:
  int statementNumber;
};
