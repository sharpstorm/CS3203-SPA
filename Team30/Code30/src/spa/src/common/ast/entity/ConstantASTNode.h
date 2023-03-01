#pragma once

#include <string>
#include <memory>
#include "common/ast/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  void accept(Extractor* e);
  string toString();
};