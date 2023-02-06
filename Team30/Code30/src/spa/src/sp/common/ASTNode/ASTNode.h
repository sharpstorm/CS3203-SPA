#pragma once

#include <vector>
#include <string>
#include <memory>
#include "sp/extractor/Extractor.h"

using std::vector, std::string, std::shared_ptr;

class Extractor;

class ASTNode {
 public:
  virtual void accept(shared_ptr<Extractor> e) = 0;
  virtual string toString() = 0;
  virtual vector<shared_ptr<ASTNode>> getChildren();
  virtual void setChild(int index, shared_ptr<ASTNode> node);
  virtual void addChild(shared_ptr<ASTNode> node);
 protected:
  vector<shared_ptr<ASTNode>> children;
};
