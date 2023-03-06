#include "PQLAssignPatternContext.h"

#include <memory>
#include <utility>
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "common/pattern/PatternConverter.h"

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

  ExpressionSequencePtr sequence = buildPostfix(secondArg.get());
  ExpressionArgumentPtr exprArg = make_unique<ExpressionArgument>(
      std::move(sequence), secondArg->allowsPartial());

  return make_unique<AssignPatternClause>(
      *synonym, std::move(firstArg), std::move(exprArg));
}

ExpressionSequencePtr PQLAssignPatternContext::buildPostfix(
    IntermediateExpressionArgument* arg) {
  IASTPtr astTree;
  try {
    astTree = std::move(arg->parse(exprParser));
  } catch (...) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_INVALID_PATTERN);
  }

  ExpressionSequencePtr expr = PatternConverter::convertASTToPostfix(
      astTree.get());
  if (expr == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_INVALID_PATTERN);
  }

  return std::move(expr);
}
