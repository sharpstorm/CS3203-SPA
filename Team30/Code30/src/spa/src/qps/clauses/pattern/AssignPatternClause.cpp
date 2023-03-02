#include <utility>
#include <unordered_set>

#include "AssignPatternClause.h"
#include "qps/clauses/arguments/SynonymArgument.h"

using std::unordered_set;

AssignPatternClause::AssignPatternClause(PQLQuerySynonym assignSynonym,
                                         ClauseArgumentPtr leftArg,
                                         ExpressionArgumentPtr rightArg):
    assignSynonym(assignSynonym),
    leftArgument(std::move(leftArg)),
    rightArgument(std::move(rightArg)) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = StmtRef{StmtType::Assign, 0};
  EntityRef rightVariable = leftArgument->toEntityRef();
  QueryResult<int, string> modifiesResult =
      pkbQueryHandler->queryModifies(leftStatement, rightVariable);

  ClauseArgumentPtr synArg = make_unique<SynonymArgument>(assignSynonym);
  QueryResult<int, string> assignResult;
  if (rightArgument->isWildcard()) {
    return Clause::toQueryResult(synArg.get(), leftArgument.get(),
                                 modifiesResult);
  } else {
    // Go through all the line numbers
    for (auto& it : modifiesResult.pairVals) {
      // Call assigns to retrieve the node
      StmtRef assignRef = StmtRef{StmtType::Assign, it.first};
      QueryResult<int, shared_ptr<IASTNode>> nodes =
          pkbQueryHandler->queryAssigns(assignRef);

      shared_ptr<IASTNode> lineRoot = *nodes.secondArgVals.begin();
      // DFS to match
      // If successful, add to query result table
      if ((!rightArgument->allowsPartial() && matchExact(lineRoot))
          || rightArgument->allowsPartial() && matchPartial(lineRoot)) {
        assignResult.add(it.first, it.second);
      }
    }
  }

  // Convert to PQLQueryResult
  return Clause::toQueryResult(synArg.get(), leftArgument.get(), assignResult);
}

SynonymList AssignPatternClause::getUsedSynonyms() {
  SynonymList result{ assignSynonym.getName() };
  if (leftArgument->isNamed()) {
    result.push_back(leftArgument->getName());
  }
  return result;
}

bool AssignPatternClause::validateArgTypes(VariableTable *variables) {
  if (!assignSynonym.isType(PQL_SYN_TYPE_ASSIGN)) {
    return false;
  }

  return leftArgument->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);
}

bool AssignPatternClause::matchPartial(shared_ptr<IASTNode> node) {
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

bool AssignPatternClause::matchExact(shared_ptr<IASTNode> rootNode) {
  if (rootNode->getType() != ASTNODE_VARIABLE &&
      rootNode->getType() != ASTNODE_CONSTANT) {
    return false;
  }
  return rootNode->getValue() == rightArgument->getPattern();
}

