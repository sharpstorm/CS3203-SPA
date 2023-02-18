#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  void accept(Extractor* e);
  string toString();
  string getValue();
};
