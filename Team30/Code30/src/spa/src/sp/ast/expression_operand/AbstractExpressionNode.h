#pragma once

#include <string>
#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

using std::string;

class AbstractExpressionNode: public BinaryASTNode {
 public:
  virtual ~AbstractExpressionNode() = default;
  void accept(IExtractor* e);
  string toString();
  string getValue();

 protected:
  AbstractExpressionNode(ASTNodeType type, string identifier, string symbol);

 private:
  string identifier;
  string symbolicRepr;
};
