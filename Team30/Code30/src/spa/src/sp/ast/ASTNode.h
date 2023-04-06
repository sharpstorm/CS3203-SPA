#pragma once

#include <vector>
#include <memory>

#include "sp/extractor/IExtractor.h"
#include "common/ast/IASTNode.h"

using std::vector, std::unique_ptr;

class IExtractor;
class ASTNode;

typedef unique_ptr<ASTNode> ASTNodePtr;
typedef vector<ASTNode *> ASTNodeRefList;

class ASTNode : public IASTNode {
 public:
  explicit ASTNode(ASTNodeType type);
  ASTNode(ASTNodeType type, const ASTNodeValue &value);

  IASTNode *getChild(int index) const override;
  ASTNode *getMutableChild(int index) const;
  int getChildCount() const override;
  ASTNodeType getType() const override;
  ASTNodeValue getValue() const override;
  bool isEquals(IASTNode *other) const override;

  virtual const ASTNodeRefList getChildren() const;
  virtual void setChild(int index, ASTNodePtr node);
  virtual void addChild(ASTNodePtr node);

  virtual void accept(IExtractor *e) const {}
  virtual void leave(IExtractor *e) const {}

 protected:
  ASTNodeValue value;
  ASTNodeType type;
  vector<ASTNodePtr> children;
};
