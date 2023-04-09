#pragma once

#include "common/Types.h"
#include "CFGBaseResultWriter.h"

template<class T, TypePredicate<T> typePredicate>
class CFGPairResultWriter : public BaseCFGResultWriter<T> {
 private:
  StmtValue left;
  StmtType targetType;

 public:
  CFGPairResultWriter(CFG *cfg, const T *closure,
                      StmtTransitiveResult *result) :
      BaseCFGResultWriter<T>(cfg, closure, result) {}

  void setParams(StmtValue val, StmtType type) {
    left = val;
    targetType = type;
  }

  bool writeRight(StmtValue stmt) const final {
    if (typePredicate(*closure, targetType, stmt)) {
      result->addPair(left, stmt);
    }

    return true;
  }

  void setLeft(StmtValue val) final {
    left = val;
  }

  StmtValue getLeft() const {
    return left;
  }
};
