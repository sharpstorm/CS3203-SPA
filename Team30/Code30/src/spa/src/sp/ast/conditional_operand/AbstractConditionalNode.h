#pragma once

#include <string>
#include "sp/extractor/IExtractor.h"
#include "sp/ast/BinaryASTNode.h"

using std::string;

class AbstractConditionalNode: public BinaryASTNode {
 public:
  virtual ~AbstractConditionalNode() = default;
  void accept(IExtractor* e) override;

 protected:
  explicit AbstractConditionalNode(ASTNodeType type) :
      BinaryASTNode(type) {}

 private:
  string identifier;
};
