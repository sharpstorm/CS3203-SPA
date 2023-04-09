#pragma once

#include "common/Types.h"
#include "CFGBaseResultWriter.h"

template<class T, TypePredicate<T> typePredicate>
class CFGSingleResultWriter : public BaseCFGResultWriter<T> {
 private:
  StmtValue pivot;
  StmtType targetType;

 public:
  CFGSingleResultWriter(CFG *cfg, const T *closure,
                        StmtTransitiveResult *result) :
      BaseCFGResultWriter<T>(cfg, closure, result) {}

  void setParams(StmtValue val, StmtType type) {
    pivot = val;
    targetType = type;
  }

  bool writeLeft(StmtValue stmt) const final {
    if (typePredicate(*closure, targetType, stmt)) {
      result->addLeft(stmt);
    }

    return true;
  }

  bool writeRight(StmtValue stmt) const final {
    if (typePredicate(*closure, targetType, stmt)) {
      result->addRight(stmt);
    }

    return true;
  }

  void setLeft(StmtValue val) final {
    pivot = val;
  }

  StmtValue getPivot() const {
    return pivot;
  }
};
