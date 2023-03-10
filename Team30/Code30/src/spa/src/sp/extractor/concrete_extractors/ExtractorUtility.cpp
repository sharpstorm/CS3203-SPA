#include "ExtractorUtility.h"

void ExtractorUtility::getExprVariables(unordered_set<string>* v, ASTNode* node) {
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
    getExprVariables(v, node->getChildren()[0]);
  }
  if (node->getChildren()[1] != nullptr) {
    getExprVariables(v, node->getChildren()[1]);
  }
}

bool ExtractorUtility::setContains(unordered_set<std::string> *v, const std::string &x) {
  return v->find(x) != v->end();
}