#pragma once

#include <string>
#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

using std::string;

class AbstractConditionalNode: public BinaryASTNode {
 public:
  virtual ~AbstractConditionalNode() = default;
  void accept(IExtractor* e);
  string toString();

 protected:
  AbstractConditionalNode(ASTNodeType type, string identifier) :
      BinaryASTNode(type), identifier(identifier) {}

 private:
  string identifier;
};
