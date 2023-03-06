#include "PatternConverter.h"
#include "TrieBuilder.h"

#include <memory>
#include <utility>

using std::make_unique;

ExpressionSequencePtr PatternConverter::convertASTToPostfix(IAST* tree) {
  if (tree == nullptr || tree->getRoot() == nullptr) {
    return nullptr;
  }

  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot().get(), result.get());
  return std::move(result);
}

PatternTriePtr PatternConverter::convertASTToTrie(IASTNodePtr tree) {
  return TrieBuilder(tree).build();
}

void PatternConverter::traversePostfix(IASTNode *node,
                                       ExpressionSequence *output) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i).get(), output);
  }

  output->push_back(node->getValue());
}
