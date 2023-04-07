#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(const ASTNodeValue &value);
  void accept(IExtractor* e) const override;
};
