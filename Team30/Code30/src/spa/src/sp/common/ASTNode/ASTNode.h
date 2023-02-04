#pragma once

#include <vector>
#include <string>
#include <memory>

using std::vector, std::string, std::shared_ptr;

class ASTNode {
 public:
  virtual ~ASTNode() = default;
  virtual void accept() = 0;
  virtual string toString() = 0;
  vector<shared_ptr<ASTNode>> getChildren();

 protected:
  vector<shared_ptr<ASTNode>> children;
};
