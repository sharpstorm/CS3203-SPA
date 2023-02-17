#pragma once

#include <memory>
#include <vector>

#include "sp/extractor/AbstractExtractor.h"
#include "common/ASTNode/VariableASTNode.h"
#include "common/ASTNode/ConstantASTNode.h"

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter* pkbWriter);
  void visit(AssignNode node);
 private:
  void addPattern(int x, shared_ptr<IASTNode> node);
  PkbWriter* pkbWriter;
};
