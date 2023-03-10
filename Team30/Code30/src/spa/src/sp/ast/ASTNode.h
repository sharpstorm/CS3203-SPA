#pragma once

#include <vector>
#include <string>
#include <memory>

#include "sp/extractor/IExtractor.h"
#include "common/ast/IASTNode.h"

using std::vector, std::string, std::unique_ptr;

class IExtractor;
class ASTNode;

typedef unique_ptr<ASTNode> ASTNodePtr;

class ASTNode : public IASTNode {
 public:
  explicit ASTNode(ASTNodeType type);
  ASTNode(ASTNodeType type, string value);

  IASTNode* getChild(int index) override;
  int getChildCount() override;
  ASTNodeType getType() override;
  string getValue() override;
  bool isEquals(IASTNode* other) override;

  virtual string toString() = 0;
  virtual vector<ASTNode*> getChildren();
  virtual void setChild(int index, ASTNodePtr node);
  virtual void addChild(ASTNodePtr node);

  virtual void accept(IExtractor* e) {}
  virtual void leave(IExtractor* e) {}

 protected:
  string value;
  ASTNodeType type;
  vector<ASTNodePtr> children;
};
