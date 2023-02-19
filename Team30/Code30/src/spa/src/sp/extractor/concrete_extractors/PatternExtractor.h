#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "common/ASTNode/entity/VariableASTNode.h"
#include "common/ASTNode/entity/ConstantASTNode.h"

using std::shared_ptr;

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter* pkbWriter);
  void visit(AssignNode* node);
 private:
  void addPattern(int x, shared_ptr<IASTNode> node);
  PkbWriter* pkbWriter;
};
