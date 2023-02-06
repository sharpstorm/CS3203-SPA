#pragma once

#include <string>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(string value);
  ~VariableASTNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
 private:
  string value;
};
