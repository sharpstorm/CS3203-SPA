#pragma once

#include <vector>
#include <string>
#include <memory>

using std::vector, std::string, std::shared_ptr;

class Extractor;

class ASTNode {
 public:
  virtual void accept(const Extractor* e) = 0;
  virtual string toString() = 0;
  virtual vector<shared_ptr<ASTNode>> getChildren() = 0;
  virtual void setChild(int index, shared_ptr<ASTNode> node) = 0;
  virtual void addChild(shared_ptr<ASTNode> node) = 0;
 protected:
  vector<shared_ptr<ASTNode>> children;
};
