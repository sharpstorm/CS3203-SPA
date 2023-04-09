#include "ParentTTableManager.h"

void ParentTTableManager::insert(StmtValue arg1, StmtValue arg2) {
  // keep only maxChild
  if (table->get(arg1) < arg2) {
    table->insert(arg1, arg2);
  }
  reverseTable->insert(arg2, arg1);
}

StmtValue ParentTTableManager::getByFirstArg(StmtValue arg1) const {
  return table->get(arg1);
}

StmtValue ParentTTableManager::getLastSibling(StmtValue stmt) const {
  const StmtValue lastVal = followsTable->getLastValue(stmt);
  if (lastVal == 0) {
    return stmt;
  } else {
    return lastVal;
  }
}

const StmtValueSet ParentTTableManager::getBySecondArg(StmtValue arg2) const {
  // find max sibling
  auto maxSibling = getLastSibling(arg2);
  return reverseTable->get(maxSibling);
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    StmtValue arg1, StmtValue arg2,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  auto maxChild = getByFirstArg(arg1);
  if (arg1 < arg2 && arg2 <= maxChild) {
    resultBuilder->add(arg1, arg2);
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    Predicate<StmtValue> arg1Predicate, const StmtValueSet &arg2Values,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  QueryResult<StmtValue, StmtValue> result;
  for (auto arg2 : arg2Values) {
    auto arg1Values = getBySecondArg(arg2);
    for (auto arg1 : arg1Values) {
      if (arg1Predicate(arg1)) {
        resultBuilder->add(arg1, arg2);
      }
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    const StmtValueSet &arg1Values, Predicate<StmtValue> rightPredicate,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto arg1 : arg1Values) {
    auto maxChild = getByFirstArg(arg1);
    if (maxChild == 0) {
      continue;
    }
    for (int i = arg1 + 1; i <= maxChild; i++) {
      if (rightPredicate(i)) {
        resultBuilder->add(arg1, i);
      }
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::hasRelation(
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  if (!table->isEmpty()) {
    resultBuilder->setIsNotEmpty();
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::rightWildcardQuery(
    const unordered_set<StmtValue> &leftArgValues,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto leftArg : leftArgValues) {
    auto rightArg = getByFirstArg(leftArg);
    if (rightArg != 0) {
      resultBuilder->addLeft(leftArg);
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::leftWildcardQuery(
    const unordered_set<StmtValue> &rightArgValues,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  QueryResult<StmtValue, StmtValue> result;
  for (auto rightArg : rightArgValues) {
    auto leftArg = getBySecondArg(rightArg);
    if (!leftArg.empty()) {
      resultBuilder->addRight(rightArg);
    }
  }
  return resultBuilder->getResult();
}
