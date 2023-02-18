#include <utility>
#include <iostream>

#include "AssignPatternClause.h"

using std::move;

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
  // Go through all the line numbers
  for (auto& it: modifiesResult.pairVals) {
    // Call assigns to retrieve the node
    StmtRef assignRef = StmtRef{StmtType::Assign, it.first};
    QueryResult<int, shared_ptr<IASTNode>> nodes =
        pkbQueryHandler->queryAssigns(assignRef);

    shared_ptr<IASTNode> lineRoot = *nodes.secondArgVals.begin();
    // DFS to match
    // If successful, add to query result table
    shared_ptr<string> substring(new string());
    if (findExpression(substring, lineRoot)) {
      assignResult.insert(it.first);
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

string AssignPatternClause::getNodeString(shared_ptr<IASTNode> node) {
  switch (node->getType())  {
    case ASTNODE_PLUS:
      return "+";
    case ASTNODE_MINUS:
      return "-";
    case ASTNODE_TIMES:
      return "*";
    case ASTNODE_DIV:
      return "/";
    case ASTNODE_MOD:
      return "%";
    default:
      return node->getValue();
  }
}

bool AssignPatternClause::findExpression(shared_ptr<string> currentSubstring,
                                         shared_ptr<IASTNode> rootNode) {
  if (rootNode.get() == nullptr) {
    return false;
  }

  bool hasMatch = findExpression(currentSubstring, rootNode->getChild(0));
  currentSubstring->append(getNodeString(rootNode));

  int matchIdx = currentSubstring->find(patternPhrase);
  hasMatch |= matchIdx != string::npos;

  // Even if there is a match, it must start from 0
  if (!allowPartial && (!hasMatch || matchIdx != 0)) {
    return false;
  }

  // If partial expression already found in leftsubtree + root, early terminate
  if (allowPartial && hasMatch) {
    return true;
  }

  hasMatch |= findExpression(currentSubstring, rootNode->getChild(1));
  return hasMatch;
}

