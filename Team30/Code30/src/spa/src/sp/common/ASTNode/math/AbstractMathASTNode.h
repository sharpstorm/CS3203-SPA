#pragma once

#include <string>
#include <memory>
#include "../AbstractBinaryASTNode.h"

using std::string, std::shared_ptr;

class AbstractMathASTNode: public AbstractBinaryASTNode {
 public:
  virtual ~AbstractMathASTNode() = default;
  void accept();
  string toString();
  int getPriority();

 protected:
  AbstractMathASTNode(string repr);

 private:
  string repr;
};
