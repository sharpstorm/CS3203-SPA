#pragma once

#include <string>
#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"

using std::string;

typedef int LineNumber;

class StatementASTNode: public ASTNode {
 private:
  LineNumber line;

 public:
  LineNumber getLineNumber() { return line; }

 protected:
  StatementASTNode(ASTNodeType type, string value, LineNumber line):
      ASTNode(type, value), line(line) {}
};
