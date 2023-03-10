#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  void accept(IExtractor* e);
  string toString();
};
