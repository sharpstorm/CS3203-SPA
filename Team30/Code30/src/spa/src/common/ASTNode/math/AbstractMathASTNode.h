#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/AbstractBinaryASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class AbstractMathASTNode: public AbstractBinaryASTNode {
 public:
  virtual ~AbstractMathASTNode() = default;
  void accept(shared_ptr<Extractor> e);
  void leave(shared_ptr<Extractor> e);
  string toString();

 protected:
  explicit AbstractMathASTNode(string repr);

 private:
  string repr;
};
