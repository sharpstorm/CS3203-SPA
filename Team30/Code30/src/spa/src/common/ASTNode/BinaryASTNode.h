#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class BinaryASTNode: public ASTNode {
 public:
  virtual ~BinaryASTNode() = default;
  virtual void accept(shared_ptr<Extractor> e);
  virtual string toString();
  void setLeftChild(shared_ptr<ASTNode> left);
  void setRightChild(shared_ptr<ASTNode> right);
  shared_ptr<ASTNode> getLeftChild();
  shared_ptr<ASTNode> getRightChild();
 protected:
  BinaryASTNode(ASTNodeType type, string value);
};
