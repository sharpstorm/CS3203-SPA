#pragma once

#include <string>
#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"
#include "sp/ast/entity/ProcedureNode.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter* pkbWriter);
  void visit(ProcedureNode* node);
  void visit(PrintNode* node);
  void visit(AssignNode* node);
  void visit(WhileNode* node);
  void visit(IfNode* node);
  void visit(ReadNode* node);
  void visit(CallNode* node);
  void visit(VariableASTNode* node);
  void visit(ConstantASTNode* node);
 private:
  PkbWriter* pkbWriter;
  int procStart;
  string procNameCache;
};
