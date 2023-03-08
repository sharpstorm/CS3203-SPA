#include <utility>
#include <memory>
#include <unordered_set>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"

using std::unordered_set, std::make_unique;

AssignPatternClause::AssignPatternClause(const PQLQuerySynonym &assignSynonym,
                                         ClauseArgumentPtr leftArg,
                                         ExpressionArgumentPtr rightArg):
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    PkbQueryHandler* pkbQueryHandler) {
  StmtRef leftStatement = {StmtType::Assign, 0};
  EntityRef rightVariable = leftArg->toEntityRef();
  QueryResult<int, string> modifiesResult =
      pkbQueryHandler->queryModifies(leftStatement, rightVariable);

  ClauseArgumentPtr synArg = make_unique<SynonymArgument>(synonym);
  QueryResult<int, string> assignResult;
  if (rightArgument->isWildcard()) {
    return Clause::toQueryResult(synArg.get(), leftArg.get(),
                                 modifiesResult);
  } else {
    // Go through all the line numbers
    for (auto& it : modifiesResult.pairVals) {
      // Call assigns to retrieve the node
      StmtRef assignRef = {StmtType::Assign, it.first};
      QueryResult<int, PatternTrie*> nodes =
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
  }

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synArg.get(), leftArg.get(), assignResult);
}

