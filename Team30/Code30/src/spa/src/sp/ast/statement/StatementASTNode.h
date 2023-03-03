#pragma once

#include <string>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class StatementASTNode: public ASTNode {
 private:
  int lineNumber;

 public:
  int getLineNumber() { return lineNumber; }

 protected:
  StatementASTNode(ASTNodeType type, string value, int lineNumber):
      ASTNode(type, value), lineNumber(lineNumber) {}
};
