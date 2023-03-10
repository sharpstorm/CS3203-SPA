#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "sp/ast/entity/VariableASTNode.h"
#include "sp/ast/entity/ConstantASTNode.h"

using std::shared_ptr;

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter* pkbWriter);
  void visitAssign(AssignNode* node) override;
  void visitWhile(WhileNode* node) override;
  void visitIf(IfNode* node) override;
 private:
  void addPattern(int x, shared_ptr<IASTNode> node);
  PkbWriter* pkbWriter;
};
