#pragma once

#include <string>
#include "sp/extractor/Extractor.h"
#include "common/ast/BinaryASTNode.h"

using std::string;

class AbstractExpressionNode: public BinaryASTNode {
 public:
  virtual ~AbstractExpressionNode() = default;
  void accept(Extractor* e);
  string toString();

 protected:
  AbstractExpressionNode(ASTNodeType type, string identifier) :
      BinaryASTNode(type), identifier() {}

 private:
  string identifier;
};
