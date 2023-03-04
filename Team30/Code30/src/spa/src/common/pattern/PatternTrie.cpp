#include "PatternTrie.h"

#include <utility>

PatternTrie::PatternTrie(IASTPtr ast): ast(std::move(ast)) {}

bool PatternTrie::isMatchFull(ExpressionSequence *sequence) {
  if (ast == nullptr || ast->getRoot() == nullptr) {
    return false;
  }

  IASTNodePtr root = ast->getRoot();
  return isMatchNode(root, sequence);
}

bool PatternTrie::isMatchPartial(ExpressionSequence *sequence) {
  if (ast == nullptr || ast->getRoot() == nullptr) {
    return false;
  }

  return traverseForMatch(ast->getRoot(), sequence);
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
