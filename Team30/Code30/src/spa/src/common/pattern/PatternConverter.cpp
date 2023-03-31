#include "PatternConverter.h"

#include <memory>
#include <utility>

#include "TrieBuilder.h"
#include "pkb/writers/PkbWriter.h"

using std::make_unique;

ExpressionSequencePtr PatternConverter::convertASTToPostfix(
    IAST* tree, QueryExecutorAgent agent) {
  if (tree == nullptr || tree->getRoot() == nullptr) {
    return nullptr;
  }

  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot(), result.get(), agent);
  return std::move(result);
}

PatternTriePtr PatternConverter::
convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter) {
  return TrieBuilder(tree, pkbWriter).build();
}

void PatternConverter::traversePostfix(IASTNode *node,
                                       ExpressionSequence *output,
                                       QueryExecutorAgent agent) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i), output, agent);
  }

  output->push_back(indexFromNode(node, agent));
}
uint16_t PatternConverter::indexFromNode(IASTNode *node,
                                         QueryExecutorAgent agent) {
  switch (node->getType()) {
    case ASTNODE_VARIABLE:
      return agent->getIndexOfVariable(node->getValue());
    case ASTNODE_CONSTANT:
      return agent->getIndexOfConstant(node->getValue()) | TRIE_CONST_MASK;
    case ASTNODE_PLUS:
      return TRIE_PLUS;
    case ASTNODE_MINUS:
      return TRIE_MINUS;
    case ASTNODE_TIMES:
      return TRIE_TIMES;
    case ASTNODE_DIV:
      return TRIE_DIV;
    case ASTNODE_MOD:
      return TRIE_MOD;
    default:
      return TRIE_INVALID_SYMBOL;
  }
}

