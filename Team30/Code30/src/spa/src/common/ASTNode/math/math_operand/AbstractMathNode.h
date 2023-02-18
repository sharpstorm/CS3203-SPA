#pragma once

#include <string>
#include "sp/extractor/Extractor.h"
#include "common/ASTNode/BinaryASTNode.h"

using std::string, std::shared_ptr;

class AbstractMathNode: public BinaryASTNode {
 public:
  virtual ~AbstractMathNode() = default;
  void accept(shared_ptr<Extractor> e);
  string toString();

 protected:
  AbstractMathNode(ASTNodeType type, string identifier) :
      BinaryASTNode(type), identifier() {}

 private:
  string identifier;
};
