#include "PatternConverter.h"
#include "TrieBuilder.h"
#include "pkb/writers/PkbWriter.h"

#include <memory>
#include <utility>

using std::make_unique;

ExpressionSequencePtr PatternConverter::convertASTToPostfix(IAST* tree) {
  if (tree == nullptr || tree->getRoot() == nullptr) {
    return nullptr;
  }

  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot(), result.get());
  return std::move(result);
}

PatternTriePtr PatternConverter::
convertASTToTrie(IASTNode* tree, PkbWriter* pkbWriter) {
  return TrieBuilder(tree, pkbWriter).build();
}

void PatternConverter::traversePostfix(IASTNode *node,
                                       ExpressionSequence *output) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i), output);
  }

  output->push_back(node->getValue());
}
