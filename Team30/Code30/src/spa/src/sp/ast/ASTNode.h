#pragma once

#include <vector>
#include <string>
#include <memory>

#include "sp/extractor/IExtractor.h"
#include "common/ast/IASTNode.h"

using std::vector, std::string, std::shared_ptr;

class IExtractor;
class ASTNode;

typedef shared_ptr<ASTNode> ASTNodePtr;

class ASTNode : public IASTNode {
 public:
  explicit ASTNode(ASTNodeType type);
  ASTNode(ASTNodeType type, string value);
  virtual string toString() = 0;

  virtual vector<ASTNodePtr> getChildren();
  virtual void setChild(int index, ASTNodePtr node);
  virtual void addChild(ASTNodePtr node);
  shared_ptr<IASTNode> getChild(int index);
  int getChildCount();

  virtual void accept(IExtractor* e) {}
  virtual void leave(IExtractor* e) {}

  ASTNodeType getType();
  string getValue();
  bool isEquals(IASTNode* other);
 protected:
  string value;
  ASTNodeType type;
  vector<ASTNodePtr> children;
};
