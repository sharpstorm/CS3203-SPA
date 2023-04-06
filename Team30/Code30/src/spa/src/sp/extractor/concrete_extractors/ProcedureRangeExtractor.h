#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "sp/SPTypes.h"

class ProcedureRangeExtractor : public AbstractExtractor {
 public:
  explicit ProcedureRangeExtractor(PkbWriter *pkbWriter);
  void visitProcedure(const ProcedureNode *node) override;
  void visitRead(const ReadNode *node) override;
  void visitPrint(const PrintNode *node) override;
  void visitWhile(const WhileNode *node) override;
  void visitIf(const IfNode *node) override;
  void visitAssign(const AssignNode *node) override;
  void visitCall(const CallNode *node) override;
 private:
  void addProcedureRange(const ProcedureName &name,
                         LineNumber start,
                         LineNumber end);
  void updateCache(const StatementASTNode *node);
  void handleProcedure(const ProcedureName &name, const ASTNode *stmtList);
  void recurseContainer(const ASTNode *stmtList);

  PkbWriter *pkbWriter;
  LineNumber lineNumberCache;
};
