#include "FollowsTableManager.h"

void FollowsTableManager::insert(StmtValue arg1, StmtValue arg2) {
  table->insert(arg1, arg2);
  reverseTable->insert(arg2, arg1);
}

QueryResultPtr<StmtValue, StmtValue> FollowsTableManager::query(
    StmtValue arg1, StmtValue arg2,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  auto firstVal = table->getFirstValue(arg1);
  if (firstVal == arg2) {
    resultBuilder->add(arg1, arg2);
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
  for (auto arg1 : arg1Values) {
    auto arg2 = table->getFirstValue(arg1);
    if (arg2 != 0) {
      resultBuilder->addLeft(arg1);
    }
  }
  return resultBuilder->getResult();
}

QueryResultPtr<StmtValue, StmtValue> FollowsTableManager::leftWildcardQuery(
    const StmtValueSet &arg2Values,
    QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
  for (auto arg2 : arg2Values) {
    auto arg1 = reverseTable->getLastValue(arg2);
    if (arg1 != 0) {
      resultBuilder->addRight(arg2);
    }
  }
  return resultBuilder->getResult();
}
