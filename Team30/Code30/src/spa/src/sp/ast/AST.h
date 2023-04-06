#pragma once

#include <memory>

#include "sp/ast/ASTNode.h"
#include "common/ast/IAST.h"

using std::unique_ptr;

class AST : public IAST {
 public:
  explicit AST(ASTNodePtr node);
  const ASTNode *getMutableRoot() const;
  IASTNode *getRoot() const override;

 private:
  ASTNodePtr root;
};

typedef unique_ptr<AST> ASTPtr;
