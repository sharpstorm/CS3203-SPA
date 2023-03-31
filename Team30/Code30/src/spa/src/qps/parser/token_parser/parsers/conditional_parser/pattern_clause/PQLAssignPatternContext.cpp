#include "PQLAssignPatternContext.h"

#include <memory>
#include <utility>
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "common/pattern/PatternConverter.h"
#include "sp/ast/StatementListNode.h"
#include "sp/ast/AST.h"

using std::make_unique;

PQLAssignPatternContext::PQLAssignPatternContext(
    ISourceExpressionParser *exprParser): exprParser(exprParser) {
}

PatternClausePtr PQLAssignPatternContext::parse(
    PQLQuerySynonymProxy synonym,
    ClauseArgumentPtr firstArg,
    IntermediateExpressionArgumentPtr secondArg) {

  if (secondArg->isWildcard()) {
    IASTPtr wildcardRoot = make_unique<AST>(make_unique<StatementListNode>());
    return make_unique<AssignPatternClause>(
    // TODO (KwanHW): To confirm if this is ok
    synonym, std::move(firstArg), std::move(wildcardRoot), true);
  }

  IASTPtr sequence = buildPostfix(secondArg.get());

  // Convert at evaluation stage - Parse entire AST into the pattern clause
  // Takes in an IASTPter instead of the ExpressionArg
  return make_unique<AssignPatternClause>(
  synonym, std::move(firstArg), std::move(sequence), secondArg->allowsPartial());
}

IASTPtr PQLAssignPatternContext::buildPostfix(
    IntermediateExpressionArgument* arg) {
  IASTPtr astTree;
  try {
    astTree = std::move(arg->parse(exprParser));
    return astTree;
  } catch (...) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_INVALID_PATTERN);
  }
}
