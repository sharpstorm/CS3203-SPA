#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/BinaryASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class StatementASTNode: public ASTNode {
 public:
  virtual ~StatementASTNode() = default;
  virtual string toString() = 0;
  virtual void accept(shared_ptr<Extractor> e) = 0;
  virtual ASTNodeType getType() = 0;
  int lineNumber;
};
