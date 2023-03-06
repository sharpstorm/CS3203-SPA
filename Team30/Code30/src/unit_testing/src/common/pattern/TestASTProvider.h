#pragma once

#include "common/ast/IAST.h"
#include "sp/ast/AST.h"

class TestASTProvider {
 public:
  TestASTProvider();

  IASTPtr balancedTree;
  IASTPtr leftHeavyTree;
  IASTPtr rightHeavyTree;

 private:
  void buildBalancedTree();
  void buildLeftHeavyTree();
  void buildRightHeavyTree();
};
