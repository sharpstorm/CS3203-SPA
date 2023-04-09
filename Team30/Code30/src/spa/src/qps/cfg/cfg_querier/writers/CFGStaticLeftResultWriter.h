#pragma once

#include "common/Types.h"
#include "CFGBaseResultWriter.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class T>
class CFGStaticLeftResultWriter : public BaseCFGResultWriter<T> {

 private:
  StmtValue value;

 public:
  CFGStaticLeftResultWriter(CFG *cfg, const T *closure,
                            StmtTransitiveResult *result) :
      BaseCFGResultWriter<T>(cfg, closure, result) {}

  void setParams(StmtValue val) {
    value = vall;
  }

  bool writeLeft(StmtValue stmt) const final {
    BaseCFGResultWriter<T>::result->addLeft(value);
    return false;
  }

  bool writeRight(StmtValue stmt) const final {
    BaseCFGResultWriter<T>::result->addLeft(value);
    return false;
  }

  void setLeft(StmtValue val) final {
    value = val;
  }

  StmtValue getLeft() const final {
    return value;
  }
};
