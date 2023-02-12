#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/ConstantASTNode.h"

class EntityExtractor : public AbstractExtractor {
 public:
  explicit EntityExtractor(PkbWriter* pkbWriter);
  void visit(PrintNode node);
  void visit(AssignNode node);
  void visit(WhileNode node);
  void visit(IfNode node);
  void visit(ReadNode node);
 private:
  PkbWriter* pkbWriter;
};
