#pragma once

#include "sp/ast/AST.h"
#include "SPTypes.h"

class ISourceParser {
 public:
  virtual ASTPtr parseSource(const FileData &input) = 0;
  virtual ASTPtr parseExpression(const SourceExpression &expr) = 0;
};
