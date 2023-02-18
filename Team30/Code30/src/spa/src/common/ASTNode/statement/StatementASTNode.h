#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/BinaryASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class StatementASTNode: public ASTNode {
 private:
  int lineNumber;

 public:
  virtual ~StatementASTNode() = default;
  virtual string toString() = 0;
  int getLineNumber() { return lineNumber; }

 protected:
  StatementASTNode(ASTNodeType type, string value, int lineNumber);
};
