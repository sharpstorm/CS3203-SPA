#pragma once

#include "common/Types.h"
#include "CFGBaseResultWriter.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class T>
class CFGBoolResultWriter : public BaseCFGResultWriter<T> {
 private:
  StmtValue from;
  StmtValue to;

  bool writeInternal() const {
    BaseCFGResultWriter<T>::result->setNotEmpty();
    return false;
  }

 public:
  CFGBoolResultWriter(CFG *cfg, const T *closure,
                      StmtTransitiveResult *result) :
      BaseCFGResultWriter<T>(cfg, closure, result) {}

  void setParams(StmtValue newFrom, StmtValue newTo) {
    from = newFrom;
    to = newTo;
  }

  bool writeLeft(StmtValue stmt) const final {
    return writeInternal();
  }

  bool writeRight(StmtValue stmt) const final {
    return writeInternal();
  }

  bool writeBool(StmtValue stmt) const final {
    if (stmt == to) {
      BaseCFGResultWriter<T>::result->setNotEmpty();
      return false;
    }
    return true;
  }

  void setLeft(StmtValue val) final {
    from = val;
  }

  StmtValue getLeft() const final {
    return from;
  }
};
