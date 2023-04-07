#pragma once

#include <memory>

#include "ASTNode.h"

using std::unique_ptr;

class BinaryASTNode : public ASTNode {
 public:
  virtual ~BinaryASTNode() = default;
  void setLeftChild(ASTNodePtr left);
  void setRightChild(ASTNodePtr right);

 protected:
  explicit BinaryASTNode(ASTNodeType type);
  BinaryASTNode(ASTNodeType type, const ASTNodeValue &value);
};

typedef unique_ptr<BinaryASTNode> BinaryASTNodePtr;
