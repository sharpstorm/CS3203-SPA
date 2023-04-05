#include "AssignPatternChecker.h"
#include <utility>

AssignPatternChecker::AssignPatternChecker(const ExpressionArgument *expr,
                                           const StmtRef *assignRef,
                                           const EntityRef *variableRef) :
    assignRef(assignRef), variableRef(variableRef), expr(expr) {}

void AssignPatternChecker::filterModifiesInto(
    const QueryResult<StmtValue, EntityValue> *modifiesResult,
    QueryResult<StmtValue, EntityValue> *assignResult,
    const QueryExecutorAgent &agent) const {
  if (assignRef->isKnown() && variableRef->isKnown()) {
    checkBoolean(modifiesResult, assignResult, agent);
  } else if (!assignRef->isKnown() && !variableRef->isKnown()) {
    checkBoth(modifiesResult, assignResult, agent);
  } else if (variableRef->isKnown()) {
    checkStmt(modifiesResult, assignResult, agent);
  } else if (assignRef->isKnown()) {
    checkVariable(modifiesResult, assignResult, agent);
  }
}

void AssignPatternChecker::checkBoolean(
    const QueryResult<StmtValue, EntityValue> *modifiesResult,
    QueryResult<StmtValue, EntityValue> *assignResult,
    const QueryExecutorAgent &agent) const {
  if (modifiesResult->isEmpty) {
    return;
  }

  if (!checkTrie(agent, assignRef->getValue())) {
    return;
  }

  assignResult->isEmpty = false;
}

void AssignPatternChecker::checkStmt(
    const QueryResult<StmtValue, EntityValue> *modifiesResult,
    QueryResult<StmtValue, EntityValue> *assignResult,
    const QueryExecutorAgent &agent) const {
  for (const StmtValue &stmt : modifiesResult->firstArgVals) {
    if (!checkTrie(agent, stmt)) {
      continue;
    }

    assignResult->addLeft(stmt);
  }
}

void AssignPatternChecker::checkVariable(
    const QueryResult<StmtValue, EntityValue> *modifiesResult,
    QueryResult<StmtValue, EntityValue> *assignResult,
    const QueryExecutorAgent &agent) const {
  if (modifiesResult->isEmpty) {
    return;
  }

  if (!checkTrie(agent, assignRef->getValue())) {
    return;
  }

  for (const EntityValue &var : modifiesResult->secondArgVals) {
    assignResult->addRight(var);
  }
}

void AssignPatternChecker::checkBoth(
    const QueryResult<StmtValue, EntityValue> *modifiesResult,
    QueryResult<StmtValue, EntityValue> *assignResult,
    const QueryExecutorAgent &agent) const {
  if (variableRef->isWildcard()) {
    checkStmt(modifiesResult, assignResult, agent);
    return;
  }

  for (const auto &it : modifiesResult->pairVals) {
    if (!checkTrie(agent, it.first)) {
      continue;
    }

    assignResult->addPair(it.first, it.second);
  }
}

bool AssignPatternChecker::checkTrie(const QueryExecutorAgent &agent,
                                     const StmtValue &stmtNumber) const {
  // Call assigns to retrieve the node
  StmtRef assignRef = {StmtType::Assign, stmtNumber};
  auto nodes = agent->queryAssigns(assignRef);
  PatternTrie *lineRoot = *nodes->secondArgVals.begin();
  return isTrieMatch(lineRoot);
}

bool AssignPatternChecker::isTrieMatch(PatternTrie *lineRoot) const {
  bool isPartialMatch = expr->allowsPartial()
      && lineRoot->isMatchPartial(expr->getSequence());
  bool isFullMatch = !expr->allowsPartial()
      && lineRoot->isMatchFull(expr->getSequence());
  return isPartialMatch || isFullMatch;
}