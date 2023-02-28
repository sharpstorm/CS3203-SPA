#pragma once

#include <string>
#include "sp/extractor/Extractor.h"
#include "common/ast/BinaryASTNode.h"

using std::string;

class AbstractMathNode: public BinaryASTNode {
 public:
  virtual ~AbstractMathNode() = default;
  void accept(Extractor* e);
  string toString();

 protected:
  AbstractMathNode(ASTNodeType type, string identifier) :
      BinaryASTNode(type), identifier() {}

 private:
  string identifier;
};
