#pragma once

#include <string>
#include <memory>
#include "sp/extractor/Extractor.h"
#include "common/ASTNode/BinaryASTNode.h"

using std::string, std::shared_ptr;

class AbstractConditionalNode: public BinaryASTNode {
 public:
  virtual ~AbstractConditionalNode() = default;
  void accept(Extractor* e) {
    e->visit(this);
  }
  void leave(shared_ptr<Extractor> e) {}
 protected:
  AbstractConditionalNode(ASTNodeType type, string value) :
      BinaryASTNode(type, value) {}
};
