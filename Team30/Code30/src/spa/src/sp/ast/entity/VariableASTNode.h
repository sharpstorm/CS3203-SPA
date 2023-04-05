#pragma once

#include <string>
#include <memory>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(string value);
  void accept(IExtractor* e) override;
};
