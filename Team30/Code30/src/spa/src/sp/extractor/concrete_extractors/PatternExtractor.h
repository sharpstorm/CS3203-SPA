#pragma once

#include <memory>

#include "sp/extractor/AbstractExtractor.h"
#include "common/ast/entity/VariableASTNode.h"
#include "common/ast/entity/ConstantASTNode.h"

using std::shared_ptr;

class PatternExtractor : public AbstractExtractor {
 public:
  explicit PatternExtractor(PkbWriter* pkbWriter);
  void visit(AssignNode* node);
 private:
  void addPattern(int x, shared_ptr<IASTNode> node);
  PkbWriter* pkbWriter;
};
