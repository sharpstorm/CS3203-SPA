#include "WithClauseEvaluator.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"
#include "qps/clauses/Clause.h"

constexpr SynStmtMapExtractor<EntityValue, EntityValue, StmtList> keyExtractor =
    [](const EntityValue* first, const StmtList* second) -> const EntityValue* {
      return first;
    };

constexpr SynStmtMapExtractor<StmtList, EntityValue, StmtList> valueExtractor =
    [](const EntityValue* first, const StmtList* second) -> const StmtList* {
      return second;
    };

WithClauseEvaluator::WithClauseEvaluator(const QueryExecutorAgent &agent,
                                         WithArgument* leftArg,
                                         WithArgument* rightArg):
    agent(agent),
    leftArg(leftArg),
    rightArg(rightArg),
    result(new PQLQueryResult()) {}

PQLQueryResult *WithClauseEvaluator::evaluate() {
  if (isEmptyResult()) {
    return result;
  }

  if (leftArg->doesReturnInteger()) {
    evaluateOnIntAttributes();
  } else {
    evaluateOnStringAttributes();
  }
  return result;
}

void WithClauseEvaluator::evaluateOnIntAttributes() {
  if (leftArg->isSynType(PQL_SYN_TYPE_CONSTANT)) {
    evaluateOnStmtConst(leftArg, rightArg);
  } else if (rightArg->getSynType() == PQL_SYN_TYPE_CONSTANT) {
    evaluateOnStmtConst(rightArg, leftArg);
  } else {
    evaluateOnStmtStmt();
  }
}

void WithClauseEvaluator::evaluateOnStmtStmt() {
  StmtValueSet set1 = queryForStatement(leftArg->getSyn());
  StmtValueSet set2 = queryForStatement(rightArg->getSyn());

  pair_set<StmtValue, StmtValue> queryResult;
  for (StmtValue i : set1) {
    if (set2.find(i) == set2.end()) {
      continue;
    }
    queryResult.insert({i, i});
  }
  result->add(leftArg->getSynName(), rightArg->getSynName(), queryResult);
}

void WithClauseEvaluator::evaluateOnStmtConst(WithArgument *constant,
                                              WithArgument *stmt) {
  StmtValueSet stmtSet = queryForStatement(stmt->getSyn());
  EntityValueSet constantSet = queryForEntity(constant->getSyn());
  pair_set<StmtValue, EntityValue> queryResult;
  for (StmtValue i : stmtSet) {
    EntityValue stringInt = to_string(i);
    if (constantSet.find(stringInt) == constantSet.end()) {
      continue;
    }
    queryResult.insert({i, stringInt});
  }
  result->add(stmt->getSynName(), constant->getSynName(), queryResult);
}

void WithClauseEvaluator::evaluateOnStringAttributes() {
  SynToStmtMap map1;
  SynToStmtMap map2;
  bool isLeftDefault =
      populateMap(leftArg, &map1);
  bool isRightDefault =
      populateMap(rightArg, &map2);

  if (isLeftDefault && isRightDefault) {
    auto queryResult = crossMaps<EntityValue, EntityValue>
        (&map1, &map2, keyExtractor, keyExtractor);
    addToResult(queryResult);
  } else if (!isLeftDefault && !isRightDefault) {
    auto queryResult = crossMaps<StmtValue, StmtValue>
        (&map1, &map2, valueExtractor, valueExtractor);
    addToResult(queryResult);
  } else if (isLeftDefault) {
    auto queryResult = crossMaps<EntityValue, StmtValue>
        (&map1, &map2, keyExtractor, valueExtractor);
    addToResult(queryResult);
  } else {
    auto queryResult = crossMaps<StmtValue, EntityValue>
        (&map1, &map2, valueExtractor, keyExtractor);
    addToResult(queryResult);
  }
}

constexpr PKBAttributeQuerier CallsQuerier =
    [](const QueryExecutorAgent &agent, const StmtValue &stmt) {
      return agent->getCalledDeclaration(stmt);
    };

constexpr PKBAttributeQuerier ReadQuerier =
    [](const QueryExecutorAgent &agent, const StmtValue &stmt) {
      return agent->getReadDeclarations(stmt);
    };

constexpr PKBAttributeQuerier PrintQuerier =
    [](const QueryExecutorAgent &agent, const StmtValue &stmt) {
      return agent->getPrintDeclarations(stmt);
    };

bool WithClauseEvaluator::populateMap(WithArgument *arg, SynToStmtMap *map) {
  switch (arg->getSynType()) {
    case PQL_SYN_TYPE_CALL:
      queryPkbForAttribute<CallsQuerier>(map, arg);
      return false;
    case PQL_SYN_TYPE_READ:
      queryPkbForAttribute<ReadQuerier>(map, arg);
      return false;
    case PQL_SYN_TYPE_PRINT:
      queryPkbForAttribute<PrintQuerier>(map, arg);
      return false;
    default:
      break;
  }

  EntityValueSet entSet = queryForEntity(arg->getSyn());
  for (EntityValue s : entSet) {
    map->emplace(s, StmtList{});
  }

  return true;
}

template <PKBAttributeQuerier querier>
void WithClauseEvaluator::queryPkbForAttribute(SynToStmtMap *map,
                                               WithArgument* arg) {
  StmtValueSet statements = queryForStatement(arg->getSyn());
  for (int i : statements) {
    EntityValue result = querier(agent, i);
    if (auto search = map->find(result); search != map->end()) {
      search->second.push_back(i);
    } else {
      map->emplace(result, StmtList{i});
    }
  }
}

StmtValueSet WithClauseEvaluator::queryForStatement(PQLQuerySynonymProxy syn) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  StmtRef stmtRef = clauseArg->toStmtRef();
  stmtRef = agent.transformArg(syn->getName(), stmtRef);
  if (stmtRef.isKnown()) {
    if (agent.isValid(stmtRef)) {
      return StmtValueSet{ stmtRef.getValue() };
    }
    return StmtValueSet{};
  }

  return agent->getStatementsOfType(stmtRef.getType());
}

EntityValueSet WithClauseEvaluator::queryForEntity(PQLQuerySynonymProxy syn) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  EntityRef entRef = clauseArg->toEntityRef();
  entRef = agent.transformArg(syn->getName(), entRef);
  if (entRef.isKnown()) {
    if (agent.isValid(entRef)) {
      return EntityValueSet { entRef.getValue() };
    }
    return EntityValueSet{};
  }

  return agent->getSymbolsOfType(entRef.getType());
}

bool WithClauseEvaluator::isEmptyResult() {
  bool leftReturnsInt = leftArg->doesReturnInteger();
  bool rightReturnsInt = rightArg->doesReturnInteger();
  bool isLeftIndependent = isIntegerIndependent(leftArg->getSynType());
  bool isRightIndependent = isIntegerIndependent(rightArg->getSynType());

  return leftReturnsInt && rightReturnsInt
      && !isLeftIndependent && !isRightIndependent;
}

bool WithClauseEvaluator::isIntegerIndependent(const PQLSynonymType &type) {
  return type == PQL_SYN_TYPE_STMT || type == PQL_SYN_TYPE_CONSTANT;
}


