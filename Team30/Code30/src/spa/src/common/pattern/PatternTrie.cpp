#include "PatternTrie.h"

#include <utility>

PatternTrie::PatternTrie(IASTNodePtr root): root(root) {}

bool PatternTrie::isMatchFull(ExpressionSequence *sequence) {
  if (root == nullptr) {
    return false;
  }

  return isMatchNode(root, sequence);
}

bool PatternTrie::isMatchPartial(ExpressionSequence *sequence) {
  if (root) {
    return false;
  }

  return traverseForMatch(root, sequence);
}

bool PatternTrie::traverseForMatch(IASTNodePtr curNode,
                                  ExpressionSequence *sequence) {
  if (isMatchNode(curNode, sequence)) {
    return true;
  }

  for (int i = 0; i < curNode->getChildCount(); i++) {
    if (isMatchNode(curNode->getChild(i), sequence)) {
      return true;
    }
  }

  return false;
}

bool PatternTrie::isMatchNode(IASTNodePtr node,
                              ExpressionSequence *sequence) {
  if (node->getType() != ASTNodeType::ASTNODE_VARIABLE
      && node->getType() != ASTNodeType::ASTNODE_CONSTANT) {
    return false;
  }
  return node->getValue() == sequence->at(0);
}
