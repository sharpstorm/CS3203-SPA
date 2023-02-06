#pragma once
#include <string>
#include "ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
 private:
  string value;
};
