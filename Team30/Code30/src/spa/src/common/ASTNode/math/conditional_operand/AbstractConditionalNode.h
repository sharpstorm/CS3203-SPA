#pragma once

#include <string>
#include <memory>
#include "sp/extractor/Extractor.h"
#include "common/ASTNode/BinaryASTNode.h"

using std::string, std::shared_ptr;

class AbstractConditionalNode: public BinaryASTNode {
 public:
  virtual ~AbstractConditionalNode() = default;
  void accept(Extractor* e);
  string toString();

 protected:
  AbstractConditionalNode(ASTNodeType type, string identifier) :
      BinaryASTNode(type), identifier(identifier) {}

 private:
  string identifier;
};
