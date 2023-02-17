#pragma once

#include <string>
#include <memory>
#include "sp/extractor/Extractor.h"
#include "common/ASTNode/BinaryASTNode.h"

using std::string, std::shared_ptr;

class AbstractMathNode: public BinaryASTNode {
 public:
  virtual ~AbstractMathNode() = default;
  void accept(shared_ptr<Extractor> e) {
    e->visit(*this);
  };
 protected:
  AbstractMathNode(ASTNodeType type, string value) : BinaryASTNode(type, value) {};
};