#include "AssignPatternChecker.h"

AssignPatternChecker::AssignPatternChecker(const ExpressionArgument *expr,
                                           const StmtRef *assignRef,
                                           const EntityRef *variableRef) :
    assignRef(assignRef), variableRef(variableRef), expr(expr) {}

void AssignPatternChecker::filterModifiesInto(
    const ConcreteQueryResult *modifiesResult,
    ConcreteQueryResult *assignResult,
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
    const ConcreteQueryResult *modifiesResult,
    ConcreteQueryResult *assignResult,
    const QueryExecutorAgent &agent) const {
  if (modifiesResult->empty()) {
    return;
  }

  if (!checkTrie(agent, assignRef->getValue())) {
    return;
  }

  assignResult->setNotEmpty();
}

void AssignPatternChecker::checkStmt(
    const ConcreteQueryResult *modifiesResult,
    ConcreteQueryResult *assignResult,
    const QueryExecutorAgent &agent) const {
  for (const StmtValue stmt : modifiesResult->getLeftVals()) {
    if (!checkTrie(agent, stmt)) {
      continue;
    }

    assignResult->addLeft(stmt);
  }
}

void AssignPatternChecker::checkVariable(
    const ConcreteQueryResult *modifiesResult,
    ConcreteQueryResult *assignResult,
    const QueryExecutorAgent &agent) const {
  if (modifiesResult->empty()) {
    return;
  }

  if (!checkTrie(agent, assignRef->getValue())) {
    return;
  }

  for (const EntityValue &var : modifiesResult->getRightVals()) {
    assignResult->addRight(var);
  }
}

void AssignPatternChecker::checkBoth(
    const ConcreteQueryResult *modifiesResult,
    ConcreteQueryResult *assignResult,
    const QueryExecutorAgent &agent) const {
  if (variableRef->isWildcard()) {
    checkStmt(modifiesResult, assignResult, agent);
    return;
  }

  for (const auto &it : modifiesResult->getPairVals()) {
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
  PatternTrie *lineRoot = *nodes->getRightVals().begin();
  return isTrieMatch(lineRoot);
}

bool AssignPatternChecker::isTrieMatch(PatternTrie *lineRoot) const {
  bool isPartialMatch = expr->allowsPartial()
      && lineRoot->isMatchPartial(expr->getSequence());
  bool isFullMatch = !expr->allowsPartial()
      && lineRoot->isMatchFull(expr->getSequence());
  return isPartialMatch || isFullMatch;
}
