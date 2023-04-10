/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

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
      addLeft(leftVal);
    }
    if (isRightVals) {
      addRight(rightVal);
    }
    if (isPairVals) {
      addPair(leftVal, rightVal);
    }
    setIsNotEmpty();
  }

  void setLeftVals() { isLeftVals = true; }

  void setRightVals() { isRightVals = true; }

  void setPairVals() { isPairVals = true; }

  void setIsNotEmpty() { result.setNotEmpty(); }

  void addLeft(L leftVal) { result.addLeft(leftVal); }

  void addRight(R rightVal) { result.addRight(rightVal); }

  void addPair(L leftVal, R rightVal) { result.addPair(leftVal, rightVal); }

  void setAllVals() {
    isLeftVals = true;
    isRightVals = true;
    isPairVals = true;
  }

  QueryResultPtr<L, R> getResult() {
    return make_unique<QueryResult<L, R>>(result);
  }
};
