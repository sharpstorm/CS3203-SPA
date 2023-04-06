#include "ExtractorUtility.h"

void ExtractorUtility::getExprVariables(VariableNameSet *v,
                                        const ASTNode *node) {
  if (node->getType() == ASTNodeType::ASTNODE_CONSTANT) {
    return;
  }

  if (node->getType() == ASTNodeType::ASTNODE_VARIABLE) {
    ASTNodeValue value = node->getValue();
    if (!setContains(v, value)) {
      v->insert(value);
    }
    return;
  }

  const ASTNode* leftChild = node->getMutableChild(0);
  if (leftChild != nullptr) {
    getExprVariables(v, leftChild);
  }

  const ASTNode* rightChild = node->getMutableChild(1);
  if (rightChild != nullptr) {
    getExprVariables(v, rightChild);
  }
}

bool ExtractorUtility::
setContains(VariableNameSet *v, const VariableName &x) {
  return v->find(x) != v->end();
}
