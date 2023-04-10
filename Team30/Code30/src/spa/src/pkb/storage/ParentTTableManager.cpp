#include "ParentTTableManager.h"

void ParentTTableManager::insert(StmtValue leftArg, StmtValue rightArg) {
  // keep only maxChild
  if (table->get(leftArg) < rightArg) {
    table->insert(leftArg, rightArg);
  }
  reverseTable->insert(rightArg, leftArg);
}

StmtValue ParentTTableManager::getByFirstArg(StmtValue leftArg) const {
  return table->get(leftArg);
}

StmtValue ParentTTableManager::getLastSibling(StmtValue stmt) const {
  const StmtValue lastVal = followsTable->getLastValue(stmt);
  if (lastVal == 0) {
    return stmt;
  } else {
    return lastVal;
  }
}

const StmtValueSet ParentTTableManager::getBySecondArg(
    StmtValue rightArg) const {
  // find max sibling
  auto maxSibling = getLastSibling(rightArg);
  return reverseTable->get(maxSibling);
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    StmtValue leftArg, StmtValue rightArg,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  auto maxChild = getByFirstArg(leftArg);
  if (leftArg < rightArg && rightArg <= maxChild) {
    resultBuilder->add(leftArg, rightArg);
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    Predicate<StmtValue> arg1Predicate, const StmtValueSet &arg2Values,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  QueryResult<StmtValue, StmtValue> result;
  for (auto rightArg : arg2Values) {
    auto arg1Values = getBySecondArg(rightArg);
    for (auto leftArg : arg1Values) {
      if (arg1Predicate(leftArg)) {
        resultBuilder->add(leftArg, rightArg);
      }
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> ParentTTableManager::query(
    const StmtValueSet &arg1Values, Predicate<StmtValue> rightPredicate,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto leftArg : arg1Values) {
    auto maxChild = getByFirstArg(leftArg);
    if (maxChild == 0) {
      continue;
    }
    for (int i = leftArg + 1; i <= maxChild; i++) {
      if (rightPredicate(i)) {
        resultBuilder->add(leftArg, i);
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
