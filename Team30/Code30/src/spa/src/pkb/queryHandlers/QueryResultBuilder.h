#pragma once
#include <utility>

#include "common/Types.h"
#include "memory"
#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IProvider.h"

using std::make_unique;
template <typename L, typename R>
class QueryResultBuilder {
 private:
  QueryResult<L, R> result;
  bool isLeftVals;
  bool isRightVals;
  bool isPairVals;

 public:
  QueryResultBuilder() {}

  void add(L leftVal, R rightVal) {
    if (isLeftVals) {
      result.firstArgVals.insert(leftVal);
    }
    if (isRightVals) {
      result.secondArgVals.insert(rightVal);
    }
    if (isPairVals) {
      result.pairVals.insert(std::make_pair(leftVal, rightVal));
    }
    result.isEmpty = false;
  }

  void setLeftVals(bool leftVal) { isLeftVals = leftVal; }

  void setRightVals(bool rightVal) { isRightVals = rightVal; }

  void setPairVals(bool pairVal) { isPairVals = pairVal; }

  QueryResultPtr<L, R> getResult() {
    return make_unique<QueryResult<L, R>>(result);
  }
};
