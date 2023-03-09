#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string, std::unique_ptr;

class BinaryASTNode: public ASTNode {
 public:
  virtual ~BinaryASTNode() = default;
  void setLeftChild(ASTNodePtr left);
  void setRightChild(ASTNodePtr right);
  ASTNode* getLeftChild();
  ASTNode* getRightChild();

 protected:
  explicit BinaryASTNode(ASTNodeType type);
  BinaryASTNode(ASTNodeType type, string value);
};

typedef unique_ptr<BinaryASTNode> BinaryASTNodePtr;
