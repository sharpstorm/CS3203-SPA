#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class AbstractBinaryASTNode: public ASTNode {
 public:
  virtual ~AbstractBinaryASTNode() = default;
  virtual void accept(shared_ptr<Extractor> e) = 0;
  virtual string toString() = 0;
  void setLeftChild(shared_ptr<ASTNode> left);
  void setRightChild(shared_ptr<ASTNode> right);
  shared_ptr<ASTNode> getLeftChild();
  shared_ptr<ASTNode> getRightChild();
 protected:
  AbstractBinaryASTNode();
};
