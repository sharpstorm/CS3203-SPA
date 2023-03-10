#pragma once

#include <string>
#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/ProcedureNode.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter* pkbWriter);
  void visitProcedure(ProcedureNode* node);
  void visitPrint(PrintNode* node);
  void visitAssign(AssignNode* node);
  void visitWhile(WhileNode* node);
  void visitIf(IfNode* node);
  void visitRead(ReadNode* node);
  void visitCall(CallNode* node);
  void visitVariable(VariableASTNode* node);
  void visitConstant(ConstantASTNode* node);
 private:
  PkbWriter* pkbWriter;
  string procNameCache;

  template <StmtType type, class T>
  const void addStatement(T* node) {
    pkbWriter->addStatement(node->getLineNumber(), type);
  }
};
