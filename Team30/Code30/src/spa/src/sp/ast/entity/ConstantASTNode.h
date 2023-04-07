#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(const ASTNodeValue &value);
  ~ConstantASTNode() = default;
  void accept(IExtractor *e) const override;
};
