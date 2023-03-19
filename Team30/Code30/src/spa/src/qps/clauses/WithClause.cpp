#include <utility>
#include <vector>

#include "WithClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

WithClause::WithClause(WithArgumentPtr left, WithArgumentPtr right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

constexpr SynStmtMapExtractor<string, string, StmtList> keyExtractor =
    [](const string* first, const StmtList* second) -> const string* {
      return first;
    };

constexpr SynStmtMapExtractor<StmtList, string, StmtList> valueExtractor =
    [](const string* first, const StmtList* second) -> const StmtList* {
      return second;
    };

PQLQueryResult *WithClause::evaluateOn(PkbQueryHandler *pkbQueryHandler,
                                       OverrideTable* table) {
  PQLQueryResult* result = new PQLQueryResult();

  if (isEmptyResult()) {
    return result;
  }

  if (leftArg->doesReturnInteger()) {
    evaluateOnIntAttributes(result, pkbQueryHandler, table);
  } else {
    evaluateOnStringAttributes(result, pkbQueryHandler, table);
  }
  return result;
}

bool WithClause::isEmptyResult() {
  bool leftReturnsInt = leftArg->doesReturnInteger();
  bool rightReturnsInt = rightArg->doesReturnInteger();
  bool isLeftIndependent = isIntegerIndependent(leftArg->getSynType());
  bool isRightIndependent = isIntegerIndependent(rightArg->getSynType());

  return leftReturnsInt && rightReturnsInt
      && !isLeftIndependent && !isRightIndependent;
}

void WithClause::evaluateOnIntAttributes(PQLQueryResult *result,
                                         PkbQueryHandler *pkbQueryHandler,
                                         OverrideTable *table) {
  if (leftArg->getSynType() == PQL_SYN_TYPE_CONSTANT) {
    evaluateOnStmtConst(result, pkbQueryHandler, table,
                        leftArg.get(), rightArg.get());
  } else if (rightArg->getSynType() == PQL_SYN_TYPE_CONSTANT) {
    evaluateOnStmtConst(result, pkbQueryHandler, table,
                        rightArg.get(), leftArg.get());
  } else {
    evaluateOnStmtStmt(result, pkbQueryHandler, table);
  }
}

void WithClause::evaluateOnStmtStmt(PQLQueryResult *result,
                                    PkbQueryHandler *pkbQueryHandler,
                                    OverrideTable *table) {
  StmtValueSet set1 = queryForStatement(leftArg->getSyn(),
                                        pkbQueryHandler, table);
  StmtValueSet set2 = queryForStatement(rightArg->getSyn(),
                                        pkbQueryHandler, table);
  pair_set<StmtValue, StmtValue> queryResult;
  for (StmtValue i : set1) {
    if (set2.find(i) == set2.end()) {
      continue;
    }
    queryResult.insert({i, i});
  }
  result->add(leftArg->getSynName(), rightArg->getSynName(), queryResult);
}

void WithClause::evaluateOnStmtConst(PQLQueryResult *result,
                                     PkbQueryHandler *pkbQueryHandler,
                                     OverrideTable *table,
                                     WithArgument *constant,
                                     WithArgument *stmt) {
  StmtValueSet stmtSet = queryForStatement(stmt->getSyn(),
                                           pkbQueryHandler, table);
  EntityValueSet constantSet = queryForEntity(constant->getSyn(),
                                              pkbQueryHandler, table);
  pair_set<StmtValue, EntityValue> queryResult;
  for (StmtValue i : stmtSet) {
    string stringInt = to_string(i);
    if (constantSet.find(stringInt) == constantSet.end()) {
      continue;
    }
    queryResult.insert({i, stringInt});
  }
  result->add(stmt->getSynName(), constant->getSynName(), queryResult);
}

void WithClause::evaluateOnStringAttributes(PQLQueryResult *result,
                                            PkbQueryHandler *pkbQueryHandler,
                                            OverrideTable* table) {
  SynToStmtMap map1;
  SynToStmtMap map2;
  bool isLeftDefault =
      populateMap(leftArg.get(), &map1, pkbQueryHandler, table);
  bool isRightDefault =
      populateMap(rightArg.get(), &map2, pkbQueryHandler, table);

  if (isLeftDefault && isRightDefault) {
    auto queryResult = crossMaps<string, string>
        (&map1, &map2, keyExtractor, keyExtractor);
    addToResult(result, queryResult);
  } else if (!isLeftDefault && !isRightDefault) {
    auto queryResult = crossMaps<StmtValue, StmtValue>
        (&map1, &map2, valueExtractor, valueExtractor);
    addToResult(result, queryResult);
  } else if (isLeftDefault) {
    auto queryResult = crossMaps<EntityValue, StmtValue>
        (&map1, &map2, keyExtractor, valueExtractor);
    addToResult(result, queryResult);
  } else {
    auto queryResult = crossMaps<StmtValue, EntityValue>
        (&map1, &map2, valueExtractor, keyExtractor);
    addToResult(result, queryResult);
  }
}

template <PKBAttributeQuerier querier>
void WithClause::queryPkbForAttribute(PkbQueryHandler *pkbQueryHandler,
                                      OverrideTable *table,
                                      SynToStmtMap *map,
                                      WithArgument* arg) {
  StmtValueSet statements = queryForStatement(
      arg->getSyn(), pkbQueryHandler, table);
  for (int i : statements) {
    string result = querier(pkbQueryHandler, i);
    if (auto search = map->find(result); search != map->end()) {
      search->second.push_back(i);
    } else {
      map->emplace(result, StmtList{i});
    }
  }
}

constexpr PKBAttributeQuerier CallsQuerier =
    [](PkbQueryHandler *pkbQueryHandler, const StmtValue &stmt) {
      return pkbQueryHandler->getCalledDeclaration(stmt);
    };

constexpr PKBAttributeQuerier ReadQuerier =
    [](PkbQueryHandler *pkbQueryHandler, const StmtValue &stmt) {
      return pkbQueryHandler->getReadDeclarations(stmt);
    };

constexpr PKBAttributeQuerier PrintQuerier =
    [](PkbQueryHandler *pkbQueryHandler, const StmtValue &stmt) {
      return pkbQueryHandler->getPrintDeclarations(stmt);
    };

bool WithClause::populateMap(WithArgument* arg, SynToStmtMap *map,
                             PkbQueryHandler *pkbQueryHandler,
                             OverrideTable* table) {
  switch (arg->getSynType()) {
    case PQL_SYN_TYPE_CALL:
      queryPkbForAttribute<CallsQuerier>(pkbQueryHandler, table, map, arg);
      return false;
    case PQL_SYN_TYPE_READ:
      queryPkbForAttribute<ReadQuerier>(pkbQueryHandler, table, map, arg);
      return false;
    case PQL_SYN_TYPE_PRINT:
      queryPkbForAttribute<PrintQuerier>(pkbQueryHandler, table, map, arg);
      return false;
    default:
      break;
  }

  EntityValueSet entSet = queryForEntity(arg->getSyn(), pkbQueryHandler,
                                         table);
  for (string s : entSet) {
    map->emplace(s, StmtList{});
  }

  return true;
}

bool WithClause::validateArgTypes(VariableTable *variables) {
  if (!leftArg->isAttributeValid() || !rightArg->isAttributeValid()) {
    return false;
  }
  bool leftRetInt = leftArg->doesReturnInteger();
  bool rightRetInt = rightArg->doesReturnInteger();

  // They both must return an int or a str
  return leftRetInt == rightRetInt;
}

SynonymList WithClause::getUsedSynonyms() {
  SynonymList result;
  if (leftArg->isSyn()) {
    result.push_back(leftArg->getSynName());
  }

  if (rightArg->isSyn()) {
    result.push_back(rightArg->getSynName());
  }

  return result;
}

bool WithClause::isIntegerIndependent(const PQLSynonymType &type) {
  return type == PQL_SYN_TYPE_STMT || type == PQL_SYN_TYPE_CONSTANT;
}

StmtValueSet WithClause::queryForStatement(PQLQuerySynonymProxy syn,
                                           PkbQueryHandler *pkbQueryHandler,
                                           OverrideTable* table) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  StmtRef stmtRef = clauseArg->toStmtRef();
  if (clauseArg->canSubstitute(table)) {
    OverrideTransformer transformer = table->at(syn->getName());
    stmtRef = transformer.transformArg(stmtRef);
    if (Clause::isValidRef(stmtRef, pkbQueryHandler)) {
      return StmtValueSet{ stmtRef.lineNum };
    }
    return StmtValueSet{};
  }

  return pkbQueryHandler->getStatementsOfType(stmtRef.getType());
}

EntityValueSet WithClause::queryForEntity(PQLQuerySynonymProxy syn,
                                          PkbQueryHandler *pkbQueryHandler,
                                          OverrideTable *table) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(syn);
  EntityRef entRef = clauseArg->toEntityRef();
  if (clauseArg->canSubstitute(table)) {
    OverrideTransformer transformer = table->at(syn->getName());
    entRef = transformer.transformArg(entRef);
    if (Clause::isValidRef(entRef, pkbQueryHandler)) {
      return EntityValueSet { entRef.name };
    }
    return EntityValueSet{};
  }

  return pkbQueryHandler->getSymbolsOfType(entRef.getType());
}
