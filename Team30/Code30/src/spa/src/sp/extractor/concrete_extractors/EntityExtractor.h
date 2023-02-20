#pragma once

#include "sp/extractor/AbstractExtractor.h"
#include "common/ASTNode/entity/VariableASTNode.h"
#include "common/ASTNode/entity/ConstantASTNode.h"
#include "common/ASTNode/entity/ProcedureNode.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter* pkbWriter);
  void visit(ProcedureNode* node);
  void visit(PrintNode* node);
  void visit(AssignNode* node);
  void visit(WhileNode* node);
  void visit(IfNode* node);
  void visit(ReadNode* node);
  void visit(VariableASTNode* node);
  void visit(ConstantASTNode* node);
 private:
  PkbWriter* pkbWriter;
  int procStart;
};
