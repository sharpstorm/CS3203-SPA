#pragma once

#include <string>
#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

using std::string;

class AbstractExpressionNode: public BinaryASTNode {
 public:
  virtual ~AbstractExpressionNode() = default;
  void accept(IExtractor* e) override;
  string getValue() override;

 protected:
  AbstractExpressionNode(ASTNodeType type);

 private:
  string identifier;
  string symbolicRepr;
};
