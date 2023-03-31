#include "PatternConverter.h"
#include "TrieBuilder.h"
#include "pkb/writers/PkbWriter.h"

#include <memory>
#include <utility>

using std::make_unique;

ExpressionSequencePtr PatternConverter::convertASTToPostfix(
    IAST* tree, PkbQueryHandler* handler) {
  if (tree == nullptr || tree->getRoot() == nullptr) {
    return nullptr;
  }

  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot(), result.get(), handler);
  return std::move(result);
}

PatternTriePtr PatternConverter::
convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter) {
  return TrieBuilder(tree, pkbWriter).build();
}

void PatternConverter::traversePostfix(IASTNode *node,
                                       ExpressionSequence *output,
                                       PkbQueryHandler* handler) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i), output, handler);
  }

//  output->push_back(node->getValue());
  output->push_back(indexFromNode(node, handler));
}
uint16_t PatternConverter::indexFromNode(IASTNode *node,
                                         PkbQueryHandler* handler) {
  switch (node->getType()) {
    case ASTNODE_VARIABLE:
      return handler->getIndexOfVariable(node->getValue());
    case ASTNODE_CONSTANT:
      return handler->getIndexOfConstant(node->getValue()) | TRIE_CONST_MASK;
//      EntityIdx idx = handler->getIndexOfConstant(node->getValue());
//      return idx | TRIE_CONST_MASK;
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

