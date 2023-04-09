#include "PQLAssignPatternContext.h"

#include <memory>
#include <utility>
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/errors/QPSParserSyntaxError.h"

using std::make_unique;

PQLAssignPatternContext::PQLAssignPatternContext(
    ISourceExpressionParser *exprParser) : exprParser(exprParser) {
}

PatternClausePtr PQLAssignPatternContext::parse(
    PQLQuerySynonymProxy synonym,
    ClauseArgumentPtr firstArg,
    IntermediateExpressionArgumentPtr secondArg) {

  if (secondArg->isWildcard()) {
    return make_unique<AssignPatternClause>(
        synonym, std::move(firstArg), IASTPtr(), true);
  }

  IASTPtr sequence = buildPostfix(secondArg.get());
  if (sequence == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_INVALID_PATTERN);
  }

  // Convert at evaluation stage - Parse entire AST into the pattern clause
  // Takes in an IASTPter instead of the ExpressionArg
  return make_unique<AssignPatternClause>(
      synonym,
      std::move(firstArg),
      std::move(sequence),
      secondArg->allowsPartial());
}

IASTPtr PQLAssignPatternContext::buildPostfix(
    IntermediateExpressionArgument *arg) {
  IASTPtr astTree;
  try {
    astTree = std::move(arg->parse(exprParser));
    return astTree;
  } catch (...) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_INVALID_PATTERN);
  }
}
