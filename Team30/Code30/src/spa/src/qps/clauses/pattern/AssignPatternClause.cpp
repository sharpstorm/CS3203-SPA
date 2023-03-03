#include <utility>
#include <unordered_set>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"

using std::unordered_set;

AssignPatternClause::AssignPatternClause(PQLQuerySynonym assignSynonym,
                                         ClauseArgumentPtr leftArg,
                                         ExpressionArgumentPtr rightArg):
    PatternClause(assignSynonym, std::move(leftArg), PQL_SYN_TYPE_ASSIGN),
    rightArgument(std::move(rightArg)) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    PkbQueryHandler* pkbQueryHandler) {
  StmtRef leftStatement = StmtRef{StmtType::Assign, 0};
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
      StmtRef assignRef = StmtRef{StmtType::Assign, it.first};
      QueryResult<int, IASTNodePtr> nodes =
          pkbQueryHandler->queryAssigns(assignRef);

      IASTNodePtr lineRoot = *nodes.secondArgVals.begin();
      // DFS to match
      // If successful, add to query result table
      if ((!rightArgument->allowsPartial() && matchExact(lineRoot))
          || rightArgument->allowsPartial() && matchPartial(lineRoot)) {
        assignResult.add(it.first, it.second);
      }
    }
  }

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synArg.get(), leftArg.get(), assignResult);
}

bool AssignPatternClause::matchPartial(IASTNodePtr node) {
  if (node == nullptr) {
    return false;
  }

  if (node->getType() == ASTNODE_VARIABLE ||
      node->getType() == ASTNODE_CONSTANT) {
    return node->getValue() == rightArgument->getPattern();
  }

  return matchPartial(node->getChild(0))
      || matchPartial(node->getChild(1));
}

bool AssignPatternClause::matchExact(IASTNodePtr rootNode) {
  if (rootNode->getType() != ASTNODE_VARIABLE &&
      rootNode->getType() != ASTNODE_CONSTANT) {
    return false;
  }
  return rootNode->getValue() == rightArgument->getPattern();
}

