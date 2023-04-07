#pragma once

#include "sp/ast/ASTNode.h"
#include "sp/SPTypes.h"

class ExtractorUtility {
 public:
  ExtractorUtility() {}
  void getExprVariables(VariableNameSet* v, const ASTNode* node);
 private:
  bool setContains(VariableNameSet* v, const VariableName &x);
};
