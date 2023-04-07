#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/extractor/IExtractor.h"
#include "sp/SPTypes.h"

class StatementASTNode : public ASTNode {
 private:
  LineNumber line;

 public:
  LineNumber getLineNumber() const { return line; }

 protected:
  StatementASTNode(ASTNodeType type,
                   const ASTNodeValue &value,
                   LineNumber line) :
      ASTNode(type, value), line(line) {}
};
