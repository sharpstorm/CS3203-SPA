#include "ExpressionVariableExtractor.h"


ExpressionVariableExtractor::ExpressionVariableExtractor() {}

void ExpressionVariableExtractor::visitAssign(AssignNode* node) {
  recurseExpr(&variableSetCache, node->getChildren()[1]);
}
void ExpressionVariableExtractor::visitWhile(WhileNode* node) {
  recurseExpr(&variableSetCache, node->getChildren()[0]);
}

void ExpressionVariableExtractor::visitIf(IfNode* node) {
  recurseExpr(&variableSetCache, node->getChildren()[0]);
}

unordered_set<string> ExpressionVariableExtractor::getVariableSet() {
  return variableSetCache;
}

void ExpressionVariableExtractor::recurseExpr(unordered_set<string>* v,
                                              shared_ptr<ASTNode> node) {
  if (node->getType() == ASTNodeType::ASTNODE_CONSTANT) {
    return;
  }

  if (node->getType() == ASTNodeType::ASTNODE_VARIABLE) {
    string value = node->toString();
    if (!setContains(v, value)) {
      v->insert(value);
    }
    return;
  }

  if (node->getChildren()[0] != nullptr) {
    recurseExpr(v, node->getChildren()[0]);
  }
  if (node->getChildren()[1] != nullptr) {
    recurseExpr(v, node->getChildren()[1]);
  }
}

bool ExpressionVariableExtractor::
setContains(unordered_set<string>* v, const string &x) {
  return v->find(x) != v->end();
}
