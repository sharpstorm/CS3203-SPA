#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(string value);
  ~VariableASTNode() = default;
  void accept(shared_ptr<Extractor> e);
  string toString();
  string getValue();
};
