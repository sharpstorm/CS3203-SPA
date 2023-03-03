#pragma once

#include <string>
#include "sp/extractor/Extractor.h"
#include "common/ast/BinaryASTNode.h"

using std::string;

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
