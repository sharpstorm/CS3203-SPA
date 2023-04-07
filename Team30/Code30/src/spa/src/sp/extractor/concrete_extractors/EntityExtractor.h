#pragma once

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/ProcedureNode.h"
#include "sp/SPTypes.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter *pkbWriter);
  void visitProcedure(const ProcedureNode *node) override;
  void visitPrint(const PrintNode *node) override;
  void visitAssign(const AssignNode *node) override;
  void visitWhile(const WhileNode *node) override;
  void visitIf(const IfNode *node) override;
  void visitRead(const ReadNode *node) override;
  void visitCall(const CallNode *node) override;
  void visitVariable(const VariableASTNode *node) override;
  void visitConstant(const ConstantASTNode *node) override;
 private:
  PkbWriter *pkbWriter;
  ProcedureName procNameCache;

  template<StmtType type, class T>
  const void addStatement(const T *node) {
    pkbWriter->addStatement(node->getLineNumber(), type);
  }
};
