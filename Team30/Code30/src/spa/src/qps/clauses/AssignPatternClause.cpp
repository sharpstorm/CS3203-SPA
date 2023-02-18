#include <utility>
#include <unordered_set>

#include "AssignPatternClause.h"

using std::move, std::unordered_set;

AssignPatternClause::AssignPatternClause(PQLQuerySynonym assignSynonym,
                                         ClauseArgumentPtr leftArgument,
                                         string patternPhrase,
                                         bool allowPartial):
    assignSynonym(assignSynonym),
    leftArgument(move(leftArgument)),
    patternPhrase(patternPhrase),
    allowPartial(allowPartial) {}

PQLQueryResult *AssignPatternClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = StmtRef{StmtType::Assign, 0};
  EntityRef rightVariable = leftArgument->toEntityRef();
  QueryResult<int, string> modifiesResult =
      pkbQueryHandler->queryModifies(leftStatement, rightVariable);

  unordered_set<int> assignResult;
  if (patternPhrase.empty()) {
    assignResult.insert(modifiesResult.firstArgVals.begin(),
                        modifiesResult.firstArgVals.end());
  } else {
    // Go through all the line numbers
    for (int i : modifiesResult.firstArgVals) {
      // Call assigns to retrieve the node
      StmtRef assignRef = StmtRef{StmtType::Assign, i};
      QueryResult<int, shared_ptr<IASTNode>> nodes =
          pkbQueryHandler->queryAssigns(assignRef);

      shared_ptr<IASTNode> lineRoot = *nodes.secondArgVals.begin();
      // DFS to match
      // If successful, add to query result table
      if (findExpression(lineRoot)) {
        assignResult.insert(i);
      }
    }
  }

  // Convert to PQLQueryResult
  return Clause::toQueryResult(assignSynonym.getName(),
                               assignResult);
}

bool AssignPatternClause::usesSynonym(string varName) {
  return assignSynonym.getName() == varName
      || leftArgument->isSynonymCalled(varName);
}

bool AssignPatternClause::validateArgTypes(VariableTable *variables) {
  if (!assignSynonym.isType(PQL_SYN_TYPE_ASSIGN)) {
    return false;
  }

  return leftArgument->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);
}

bool AssignPatternClause::findExpression(shared_ptr<IASTNode> rootNode) {
  if (rootNode->getType() == ASTNODE_VARIABLE ||
      rootNode->getType() == ASTNODE_CONSTANT) {
    return rootNode->getValue() == patternPhrase;
  }

  return findExpression(rootNode->getChild(0))
      || findExpression(rootNode->getChild(1));
}

