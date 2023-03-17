#include <utility>
#include <memory>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"

using std::make_unique;

AssignPatternClause::AssignPatternClause(const PQLQuerySynonym &assignSynonym,
                                         ClauseArgumentPtr leftArg,
                                         ExpressionArgumentPtr rightArg):
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
  PkbQueryHandler* pkbQueryHandler, OverrideTable* table) {
  StmtRef leftStatement = {StmtType::Assign, 0};
  EntityRef rightVariable = leftArg->toEntityRef();
  if (leftArg->isNamed() && table->find(leftArg->getName()) != table->end()) {
    OverrideTransformer overrideTransformer = table->at(leftArg->getName());
    rightVariable = overrideTransformer.transformArg(rightVariable);
  }

  QueryResult<int, string> modifiesResult =
      pkbQueryHandler->queryModifies(leftStatement, rightVariable);

  if (rightArgument->isWildcard()) {
    return Clause::toQueryResult(synonym.getName(), leftArg.get(),
                                 modifiesResult);
  }

  QueryResult<StmtValue, EntityValue> assignResult;
  // Go through all the line numbers
  for (auto& it : modifiesResult.pairVals) {
    // Call assigns to retrieve the node
    StmtRef assignRef = {StmtType::Assign, it.first};
    QueryResult<StmtValue, PatternTrie*> nodes =
        pkbQueryHandler->queryAssigns(assignRef);

    PatternTrie* lineRoot = *nodes.secondArgVals.begin();
    bool isPartialMatch = rightArgument->allowsPartial()
        && lineRoot->isMatchPartial(rightArgument->getSequence());
    bool isFullMatch = !rightArgument->allowsPartial()
        && lineRoot->isMatchFull(rightArgument->getSequence());
    if (isPartialMatch || isFullMatch) {
      assignResult.add(it.first, it.second);
    }
  }

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synonym.getName(), leftArg.get(),
                               assignResult);
}

