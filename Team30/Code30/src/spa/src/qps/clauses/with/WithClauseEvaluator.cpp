#include "WithClauseEvaluator.h"

#include <string>

#include "qps/clauses/arguments/ClauseArgumentFactory.h"

using std::to_string;

constexpr SynStmtMapExtractor<EntityValue, EntityValue, StmtList> keyExtractor =
    [](const EntityValue *first,
       const StmtList *second) -> const EntityValue * {
      return first;
    };

constexpr SynStmtMapExtractor<StmtList, EntityValue, StmtList> valueExtractor =
    [](const EntityValue *first, const StmtList *second) -> const StmtList * {
      return second;
    };

WithClauseEvaluator::WithClauseEvaluator(const QueryExecutorAgent &agent,
                                         const AttributedSynonym *leftArg,
                                         const AttributedSynonym *rightArg) :
    agent(agent),
    leftArg(leftArg),
    rightArg(rightArg),
    result(make_unique<PQLQueryResult>()) {}

PQLQueryResult *WithClauseEvaluator::evaluate() {
  PQLQueryResult *ret = result.get();

  if (isEmptyResult()) {
    result.release();
    return ret;
  }

  if (leftArg->returnsInteger()) {
    evaluateOnIntAttributes();
  } else {
    evaluateOnStringAttributes();
  }
  result.release();
  return ret;
}

void WithClauseEvaluator::evaluateOnIntAttributes() {
  if (leftArg->isType(PQL_SYN_TYPE_CONSTANT)) {
    evaluateOnStmtConst(leftArg, rightArg);
  } else if (rightArg->isType(PQL_SYN_TYPE_CONSTANT)) {
    evaluateOnStmtConst(rightArg, leftArg);
  } else {
    evaluateOnStmtStmt();
  }
}

void WithClauseEvaluator::evaluateOnStmtStmt() {
  StmtValueSet set1 = queryForStatement(leftArg->getSynProxy());
  StmtValueSet set2 = queryForStatement(rightArg->getSynProxy());

  pair_set<StmtValue, StmtValue> queryResult;
  for (const StmtValue &i : set1) {
    if (set2.find(i) == set2.end()) {
      continue;
    }
    queryResult.insert({i, i});
  }
  result->add(leftArg->getName(), rightArg->getName(), queryResult);
}

void WithClauseEvaluator::evaluateOnStmtConst(const AttributedSynonym *constant,
                                              const AttributedSynonym *stmt) {
  StmtValueSet stmtSet = queryForStatement(stmt->getSynProxy());
  EntityValueSet constantSet = queryForEntity(constant->getSynProxy());
  pair_set<StmtValue, EntityValue> queryResult;
  for (const StmtValue &i : stmtSet) {
    EntityValue stringInt = to_string(i);
    if (constantSet.find(stringInt) == constantSet.end()) {
      continue;
    }
    queryResult.insert({i, stringInt});
  }
  result->add(stmt->getName(), constant->getName(), queryResult);
}

void WithClauseEvaluator::evaluateOnStringAttributes() {
  SynToStmtMap map1;
  SynToStmtMap map2;
  bool isLeftDefault = populateMap(leftArg->getSynProxy(), &map1);
  bool isRightDefault = populateMap(rightArg->getSynProxy(), &map2);

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

bool WithClauseEvaluator::populateMap(const PQLQuerySynonymProxy &arg,
                                      SynToStmtMap *map) {
  switch (arg->getType()) {
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

  EntityValueSet entSet = queryForEntity(arg);
  for (const EntityValue &s : entSet) {
    map->emplace(s, StmtList{});
  }

  return true;
}

template<PKBAttributeQuerier querier>
void WithClauseEvaluator::queryPkbForAttribute(
    SynToStmtMap *map, const PQLQuerySynonymProxy &arg) {
  StmtValueSet statements = queryForStatement(arg);
  for (const StmtValue &i : statements) {
    EntityValue result = querier(agent, i);
    if (auto search = map->find(result); search != map->end()) {
      search->second.push_back(i);
    } else {
      map->emplace(result, StmtList{i});
    }
  }
}

StmtValueSet WithClauseEvaluator::queryForStatement(
    const PQLQuerySynonymProxy &syn) const {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  StmtRef stmtRef = clauseArg->toStmtRef();
  stmtRef = agent.transformArg(syn->getName(), stmtRef);
  if (stmtRef.isKnown()) {
    if (agent.isValid(stmtRef)) {
      return StmtValueSet{stmtRef.getValue()};
    }
    return StmtValueSet{};
  }

  return agent->getStatementsOfType(stmtRef.getType());
}

EntityValueSet WithClauseEvaluator::queryForEntity(
    const PQLQuerySynonymProxy &syn) const {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  EntityRef entRef = clauseArg->toEntityRef();
  entRef = agent.transformArg(syn->getName(), entRef);
  if (entRef.isKnown()) {
    if (agent.isValid(entRef)) {
      return EntityValueSet{entRef.getValue()};
    }
    return EntityValueSet{};
  }

  return agent->getSymbolsOfType(entRef.getType());
}

bool WithClauseEvaluator::isEmptyResult() const {
  bool leftReturnsInt = leftArg->returnsInteger();
  bool rightReturnsInt = rightArg->returnsInteger();
  bool isLeftIndependent = isIntegerIndependent(leftArg->getType());
  bool isRightIndependent = isIntegerIndependent(rightArg->getType());

  return leftReturnsInt && rightReturnsInt
      && !isLeftIndependent && !isRightIndependent;
}

bool WithClauseEvaluator::isIntegerIndependent(
    const PQLSynonymType &type) const {
  return type == PQL_SYN_TYPE_STMT || type == PQL_SYN_TYPE_CONSTANT;
}


