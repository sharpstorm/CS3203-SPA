#include <vector>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(PQLQueryResult *queryResult,
                                                 PQLQueryVariable resultVariable) {
  // Error result
  if (!queryResult->getError().empty()) {
    vector<string>* result = new vector<string>();
    result->push_back(queryResult->getError());
    return UniqueVectorPtr<string>(result);
  }

  // Static result
  if (queryResult->getIsStaticFalse()) {
    return UniqueVectorPtr<string>(new vector<string>());
  }

  PQL_VAR_NAME var = resultVariable.name;
  bool existInMap = queryResult->getFromStatementMap(var) != nullptr;
  if (!queryResult->getStatementMap().empty() && existInMap)  {
    return projectStatements(queryResult->getFromStatementMap(var));
  }

  existInMap = queryResult->getFromEntityMap(var) != nullptr;
  if (!queryResult->getEntityMap().empty() && existInMap) {
    return projectEntities(queryResult->getFromEntityMap(var));
  }

  return UniqueVectorPtr<string>(new vector<string>());
}

// NOT IN USE
//UniqueVectorPtr<string> ResultProjector::project(PQLQueryResult* queryResult) {
//  if (queryResult->getError() != "") {
//    vector<string>* result = new vector<string>();
//    result->push_back(queryResult->getError());
//    return UniqueVectorPtr<string>(result);
//  }
//
//  if (queryResult->isEntityMapEmpty()) {
//    STATEMENT_MAP statementMap = queryResult->getStatementMap();
//    return projectStatements(statementMap);
//  } else if (queryResult->isStatementMapEmpty()) {
//    ENTITY_MAP entityMap = queryResult->getEntityMap();
//    return projectEntities(entityMap);
//  }
//
//  return UniqueVectorPtr<string>(new vector<string>());
//}

UniqueVectorPtr<string> ResultProjector::projectStatements(
    StatementResult* statementResult) {
  if (statementResult == nullptr) {
    return UniqueVectorPtr<string>(new vector<string>());
  }

  vector<string>* result = new vector<string>();

    if (!statementResult->linePairs.empty()) {
      bool isLeftArg = statementResult->isLeftArg;
      for (auto stmt: statementResult->linePairs) {
        result->push_back(to_string(isLeftArg ? stmt.first : stmt.second));
      }
    }

    for (auto stmt: statementResult->lines) {
      result->push_back(to_string(stmt));
    }

  return UniqueVectorPtr<string>(result);
}

UniqueVectorPtr<string> ResultProjector::projectEntities(EntityResult* entityResult) {
  if (entityResult == nullptr) {
    return UniqueVectorPtr<string>(new vector<string>());
  }

  vector<string>* result = new vector<string>();

  if (!entityResult->entityPairs.empty()) {
   bool isLeftArg = entityResult->isLeftArg;
   for (auto &entity : entityResult->entityPairs) {
     result->push_back(isLeftArg ? to_string(entity.first) : entity.second);
   }
  } else {
    for (auto line: entityResult->lines) {
      result->push_back(to_string(line));
    }

    for (auto entity: entityResult->entities) {
      result->push_back(entity);
    }
  }

  return UniqueVectorPtr<string>(result);
}
//PQLQueryResult *ResultProjector::filterResultTo(PQLQueryResult* queryResult,
//                                                PQLQueryVariable resultVariable) {
//  PQLQueryResult* filterResult = new PQLQueryResult();
//  if (queryResult->getIsStaticFalse()) {
//    filterResult->setIsStaticFalse(queryResult->getIsStaticFalse());
//    return filterResult;
//  }
//
//  PQL_VAR_NAME var = resultVariable.name;
//  if (resultVariable.isStatementType()) {
//    StatementResult* stmtResult = queryResult->getFromStatementMap(var);
//    if (stmtResult != nullptr) {
//      filterResult->addToStatementMap(var, *stmtResult);
//    }
//  } else if (resultVariable.isEntityType()) {
//    EntityResult* entityResult = queryResult->getFromEntityMap(var);
//    if (entityResult != nullptr) {
//      filterResult->addToEntityMap(var, *entityResult);
//    }
//  }
//
////  delete queryResult;
//  filterResult->setError(queryResult.getError());
//
//  return filterResult;
//}
