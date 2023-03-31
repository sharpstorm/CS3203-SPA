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
  bool isLeftVals = false;
  bool isRightVals = false;
  bool isPairVals = false;

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

  void setLeftVals() { isLeftVals = true; }

  void setRightVals() { isRightVals = true; }

  void setPairVals() { isPairVals = true; }

  void setAllVals() {
    isLeftVals = true;
    isRightVals = true;
    isPairVals = true;
  }

  QueryResultPtr<L, R> getResult() {
    return make_unique<QueryResult<L, R>>(result);
  }
};
