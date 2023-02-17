#include <vector>
#include <memory>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(PQLQueryResult *queryResult,
                                                 PQLQuerySynonym resultVariable
) {
  UniqueVectorPtr<string> result =
      make_unique<vector<string>>(vector<string>{});
  // Error result
  if (!queryResult->getError().empty()) {
    result->push_back(queryResult->getError());
    return result;
  }

  // Static result
  if (queryResult->isStatic()) {
    return result;
  }

  PQLSynonymName var = resultVariable.getName();
  ResultTableCol col = queryResult->getSynonymCol(var);
  if (col != PQLQueryResult::NO_COL) {
    for (int i = 0; i < queryResult->getRowCount(); i++) {
      result->push_back(
          queryResult
              ->getTableRowAt(i)
              ->at(col)
              ->project());
    }
  }

  return result;
}

UniqueVectorPtr<string> ResultProjector::projectStatements(
    StatementResult* statementResult) {
  if (statementResult == nullptr) {
    return UniqueVectorPtr<string>(new vector<string>());
  }

  vector<string>* result = new vector<string>();

  if (!statementResult->linePairs.empty()) {
    bool isLeftArg = statementResult->isLeftArg;
    for (auto stmt : statementResult->linePairs) {
      result->push_back(to_string(isLeftArg ? stmt.first : stmt.second));
    }
  } else {
    for (auto stmt : statementResult->lines) {
      result->push_back(to_string(stmt));
    }
  }

  return UniqueVectorPtr<string>(result);
}

UniqueVectorPtr<string> ResultProjector::projectEntities(
    EntityResult* entityResult) {
  if (entityResult == nullptr) {
    return UniqueVectorPtr<string>(new vector<string>());
  }

  vector<string>* result = new vector<string>();

  if (!entityResult->entityPairs.empty()) {
    bool isLeftArg = entityResult->isLeftArg;
    for (auto &entity : entityResult->entityPairs) {
      result->push_back(isLeftArg ? to_string(entity.first) : entity.second);
    }
  } else if (!entityResult->procedurePairs.empty()) {
    for (auto &entity : entityResult->procedurePairs)  {
      result->push_back(entityResult->isLeftArg ?
                        entity.first : entity.second);
    }
  } else {
    for (auto line : entityResult->lines) {
      result->push_back(to_string(line));
    }

    for (auto entity : entityResult->entities) {
      result->push_back(entity);
    }
  }

  return UniqueVectorPtr<string>(result);
}
