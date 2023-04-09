#pragma once

#include "qps/cfg/CFGQuerierTypes.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/CFGQuerier.h"
#include "qps/cfg/cfg_querier/writers/ICFGResultWriter.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class T>
using TypePredicate = StmtTypePredicate<T>;

template<class T>
class BaseCFGResultWriter : public ICFGWriter {
 protected:
  CFG *cfg;
  const T *closure;
  StmtTransitiveResult *result;

  BaseCFGResultWriter(CFG *cfg, const T *closure,
                      StmtTransitiveResult *result) :
      cfg(cfg), closure(closure), result(result) {}

 public:
  const T *getClosure() const {
    return closure;
  }

  const StmtTransitiveResult *getResult() const {
    return result;
  }

  StmtValue toStmtNumber(CFGNode node) const {
    return cfg->fromCFGNode(node);
  }

  bool writeLeft(StmtValue stmt) const override {
    return false;
  }

  bool writeRight(StmtValue stmt) const override {
    return false;
  }
};
