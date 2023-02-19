#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(string value);
  void accept(Extractor* e);
  string toString();
};
