#include <utility>
#include <vector>

#include "WithClause.h"
#include "qps/clauses/arguments/PKBTypeAdapter.h"

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
    evaluateOnIntAttributes(result, pkbQueryHandler);
  } else {
    evaluateOnStringAttributes(result, pkbQueryHandler);
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
                                         PkbQueryHandler *pkbQueryHandler) {
  if (leftArg->getSynType() == PQL_SYN_TYPE_CONSTANT) {
    evaluateOnStmtConst(result, pkbQueryHandler,
                        leftArg.get(), rightArg.get());
  } else if (rightArg->getSynType() == PQL_SYN_TYPE_CONSTANT) {
    evaluateOnStmtConst(result, pkbQueryHandler,
                        rightArg.get(), leftArg.get());
  } else {
    evaluateOnStmtStmt(result, pkbQueryHandler);
  }
}

void WithClause::evaluateOnStmtStmt(PQLQueryResult *result,
                                    PkbQueryHandler *pkbQueryHandler) {
  StmtType leftType =
      PKBTypeAdapter::convertPQLSynonymToStmt(leftArg->getSynType());
  StmtType rightType =
      PKBTypeAdapter::convertPQLSynonymToStmt(rightArg->getSynType());
  StmtValueSet set1 = pkbQueryHandler->getStatementsOfType(leftType);
  StmtValueSet set2 = pkbQueryHandler->getStatementsOfType(rightType);

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
                                     WithArgument *constant,
                                     WithArgument *stmt) {
  StmtType stmtType =
      PKBTypeAdapter::convertPQLSynonymToStmt(stmt->getSynType());
  StmtValueSet stmtSet = pkbQueryHandler->getStatementsOfType(stmtType);
  EntityValueSet constantSet = pkbQueryHandler
      ->getSymbolsOfType(EntityType::Constant);

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
                                            PkbQueryHandler *pkbQueryHandler) {
  SynToStmtMap map1;
  SynToStmtMap map2;
  bool isLeftDefault =
      populateMap(leftArg->getSynType(), &map1, pkbQueryHandler);
  bool isRightDefault =
      populateMap(rightArg->getSynType(), &map2, pkbQueryHandler);

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

template <PKBAttributeQuerier querier, PQLSynonymType synType>
void WithClause::queryPkbForAttribute(PkbQueryHandler *pkbQueryHandler,
                                      SynToStmtMap *map) {
  StmtType stmtType = PKBTypeAdapter::convertPQLSynonymToStmt(synType);
  for (int i : pkbQueryHandler->getStatementsOfType(stmtType)) {
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

bool WithClause::populateMap(PQLSynonymType type, SynToStmtMap *map,
                             PkbQueryHandler *pkbQueryHandler) {
  switch (type) {
    case PQL_SYN_TYPE_CALL:
      queryPkbForAttribute<CallsQuerier,
                           PQL_SYN_TYPE_CALL>(pkbQueryHandler, map);
      return false;
    case PQL_SYN_TYPE_READ:
      queryPkbForAttribute<ReadQuerier,
                           PQL_SYN_TYPE_READ>(pkbQueryHandler, map);
      return false;
    case PQL_SYN_TYPE_PRINT:
      queryPkbForAttribute<PrintQuerier,
                           PQL_SYN_TYPE_PRINT>(pkbQueryHandler, map);
      return false;
    default:
      break;
  }

  EntityType leftEntityType = PKBTypeAdapter::convertPQLSynonymToEntity(type);
  for (string s : pkbQueryHandler->getSymbolsOfType(leftEntityType)) {
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
