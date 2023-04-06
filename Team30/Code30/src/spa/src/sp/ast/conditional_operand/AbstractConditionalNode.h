#pragma once

#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

class AbstractConditionalNode : public BinaryASTNode {
 public:
  virtual ~AbstractConditionalNode() = default;
  void accept(IExtractor *e) const override;

 protected:
  explicit AbstractConditionalNode(ASTNodeType type) :
      BinaryASTNode(type) {}
};
