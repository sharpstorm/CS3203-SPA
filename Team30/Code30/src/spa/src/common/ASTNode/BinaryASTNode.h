#pragma once

#include <string>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class BinaryASTNode: public ASTNode {
 public:
  virtual ~BinaryASTNode() = default;
  void setLeftChild(ASTNodePtr left);
  void setRightChild(ASTNodePtr right);
  ASTNodePtr getLeftChild();
  ASTNodePtr getRightChild();

 protected:
  explicit BinaryASTNode(ASTNodeType type);
  BinaryASTNode(ASTNodeType type, string value);
};
