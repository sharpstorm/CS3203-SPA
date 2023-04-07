#pragma once

#include <string>

#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

using std::string;

typedef string ASTExpressionSymbol;

class AbstractExpressionNode: public BinaryASTNode {
 public:
  virtual ~AbstractExpressionNode() = default;
  void accept(IExtractor* e) const override;
  ASTNodeValue getValue() const override;

 protected:
  AbstractExpressionNode(ASTNodeType type, const ASTExpressionSymbol &symbol);

 private:
  ASTExpressionSymbol symbolicRepr;
};
