#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"

using std::stack, std::vector, std::shared_ptr;

typedef int LineNumber;
typedef vector<LineNumber> LineNumbers;
typedef shared_ptr<LineNumbers> LineNumbersPtr;

class FollowsExtractor : public AbstractExtractor {
 public:
  explicit FollowsExtractor(PkbWriter* pkbWriter);
  void visitRead(ReadNode* node) override;
  void visitPrint(PrintNode* node) override;
  void visitAssign(AssignNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
  void visitCall(CallNode* node) override;
  void visitStmtList(StatementListNode* node) override;
  void leaveStmtList(StatementListNode* node) override;

 private:
  void addFollowsRelation(LineNumber x, LineNumber y);
  void processStatement(StatementASTNode* node);
  stack<LineNumbersPtr> stmtLstCache;
  PkbWriter* pkbWriter;
};
