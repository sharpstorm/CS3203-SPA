#pragma once

#include <vector>

#include "sp/extractor/AbstractExtractor.h"
#include "common/data_structs/VectorStack.h"
#include "sp/SPTypes.h"

typedef vector<LineNumber> LineNumbers;

class FollowsExtractor : public AbstractExtractor {
 public:
  explicit FollowsExtractor(PkbWriter* pkbWriter);
  void visitRead(const ReadNode* node) override;
  void visitPrint(const PrintNode* node) override;
  void visitAssign(const AssignNode* node) override;
  void visitWhile(const WhileNode* node) override;
  void visitIf(const IfNode* node) override;
  void visitCall(const CallNode* node) override;
  void visitStmtList(const StatementListNode* node) override;
  void leaveStmtList(const StatementListNode* node) override;

 private:
  void addFollowsRelation(LineNumber x, LineNumber y);
  void processStatement(const StatementASTNode* node);
  VectorStack<LineNumbers> stmtLstCache;
  PkbWriter* pkbWriter;
};
