#pragma once

#include <string>
#include "sp/ast/ASTNode.h"

using std::unordered_set;

class ExtractorUtility {
 public:
  ExtractorUtility(){};
  void getExprVariables(unordered_set<string>* v, ASTNode* node);
 private:
  bool setContains(unordered_set<string>* v, const string &x);
};
