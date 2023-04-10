#include "FollowsTableManager.h"

void FollowsTableManager::insert(StmtValue leftArg, StmtValue rightArg) {
  table->insert(leftArg, rightArg);
  reverseTable->insert(rightArg, leftArg);
}

QueryResultPtr<StmtValue, StmtValue> FollowsTableManager::query(
    StmtValue leftArg, StmtValue rightArg,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  auto firstVal = table->getFirstValue(leftArg);
  if (firstVal == rightArg) {
    resultBuilder->add(leftArg, rightArg);
  }

  return resultBuilder->getResult();
}

unique_ptr<IBaseIterator<StmtValue>> FollowsTableManager::getRightValIter(
    StmtValue leftArg) const {
  auto rightArg = table->getFirstValue(leftArg);
  return make_unique<ItemIterator<StmtValue>>(rightArg);
}

unique_ptr<IBaseIterator<StmtValue>> FollowsTableManager::getLeftValIter(
    StmtValue rightArg) const {
  auto leftArg = reverseTable->getLastValue(rightArg);
  return make_unique<ItemIterator<StmtValue>>(leftArg);
}

QueryResultPtr<StmtValue, StmtValue> FollowsTableManager::rightWildcardQuery(
    const StmtValueSet &arg1Values,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto leftArg : arg1Values) {
    auto rightArg = table->getFirstValue(leftArg);
    if (rightArg != 0) {
      resultBuilder->addLeft(leftArg);
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> FollowsTableManager::leftWildcardQuery(
    const StmtValueSet &arg2Values,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto rightArg : arg2Values) {
    auto leftArg = reverseTable->getLastValue(rightArg);
    if (leftArg != 0) {
      resultBuilder->addRight(rightArg);
    }
  }
  return resultBuilder->getResult();
}
