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

  if (node->getMutableChild(0) != nullptr) {
    getExprVariables(v, node->getMutableChild(0));
  }
  if (node->getMutableChild(1) != nullptr) {
    getExprVariables(v, node->getMutableChild(1));
  }
}

bool ExtractorUtility::
setContains(VariableNameSet *v, const VariableName &x) {
  return v->find(x) != v->end();
}
