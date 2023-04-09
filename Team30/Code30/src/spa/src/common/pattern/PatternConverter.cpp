#include "PatternConverter.h"

#include <memory>
#include <utility>

#include "TrieBuilder.h"
#include "pkb/writers/PkbWriter.h"
#include "PatternNodeConverter.h"

using std::make_unique;

ExpressionSequencePtr PatternConverter::convertASTToPostfix(
    IAST *tree, const PkbQueryHandler *agent) {
  if (tree == nullptr || tree->getRoot() == nullptr) {
    return nullptr;
  }

  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot(), result.get(), agent);
  return std::move(result);
}

PatternTriePtr PatternConverter::
convertASTToTrie(IASTNode *tree, PkbWriter *pkbWriter) {
  return TrieBuilder(tree, pkbWriter).build();
}

void PatternConverter::traversePostfix(IASTNode *node,
                                       ExpressionSequence *output,
                                       const PkbQueryHandler *agent) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i), output, agent);
  }

  output->push_back(indexFromNode(node, agent));
}

SymbolIdent PatternConverter::indexFromNode(IASTNode *node,
                                            const PkbQueryHandler *agent) {
  return PatternNodeConverter<const PkbQueryHandler>::indexFromNode<
      PatternConverter::queryVariable,
      PatternConverter::queryConstant>(node, agent);
}

SymbolIdent PatternConverter::queryVariable(IASTNode *node,
                                            const PkbQueryHandler *agent) {
  return agent->getIndexOfVariable(node->getValue());
}

SymbolIdent PatternConverter::queryConstant(IASTNode *node,
                                            const PkbQueryHandler *agent) {
  return agent->getIndexOfConstant(node->getValue());
}
