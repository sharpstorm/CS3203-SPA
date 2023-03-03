#include "PQLAssignPatternContext.h"

#include <memory>
#include "qps/clauses/pattern/AssignPatternClause.h"

using std::make_unique;

PQLAssignPatternContext::PQLAssignPatternContext(
    ISourceExpressionParser *exprParser): exprParser(exprParser) {
}

PatternClausePtr PQLAssignPatternContext::parse(
    PQLQuerySynonym *synonym,
    ClauseArgumentPtr firstArg,
    IntermediateExpressionArgumentPtr secondArg) {

  if (secondArg->isWildcard()) {
    return make_unique<AssignPatternClause>(
        *synonym, std::move(firstArg), make_unique<ExpressionArgument>());
  }

  IASTPtr astTree = secondArg->parse(exprParser);
  ExpressionSequencePtr sequence = buildPostfix(astTree.get());
  ExpressionArgumentPtr exprArg = make_unique<ExpressionArgument>(
      std::move(sequence), secondArg->allowsPartial());

  return make_unique<AssignPatternClause>(
      *synonym, std::move(firstArg), std::move(exprArg));
}

ExpressionSequencePtr PQLAssignPatternContext::buildPostfix(IAST* tree) {
  ExpressionSequencePtr result = make_unique<ExpressionSequence>();
  traversePostfix(tree->getRoot().get(), result.get());
  return std::move(result);
}

void PQLAssignPatternContext::traversePostfix(IASTNode *node,
                                           ExpressionSequence *output) {
  for (int i = 0; i < node->getChildCount(); i++) {
    traversePostfix(node->getChild(i).get(), output);
  }

  output->push_back(node->getValue());
}
