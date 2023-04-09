#pragma once

#include <utility>
#include "ICFGResultWriter.h"
#include "CFGBaseResultWriter.h"
#include "CFGBoolResultWriter.h"
#include "CFGSingleResultWriter.h"
#include "CFGPairResultWriter.h"
#include "CFGStaticLeftResultWriter.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class T>
class CFGResultWriterFactory {
 private:
  CFG *cfg;
  const T *closure;
  StmtTransitiveResult *result;

 public:
  CFGResultWriterFactory(CFG *cfg,
                         const T *closure,
                         StmtTransitiveResult *result) :
      cfg(cfg), closure(closure), result(result) {}

  template<TypePredicate<T> typePredicate>
  ICFGWriterPtr makeBoolWriter(StmtValue arg0,
                                   StmtValue arg1) const {
    auto writer = make_unique<CFGBoolResultWriter<T>>(cfg, closure, result);
    writer->setParams(arg0, arg1);
    return writer;
  }

  template<TypePredicate<T> typePredicate>
  ICFGWriterPtr makeLeftWriter(StmtType type0, StmtValue arg1Val) const {
    if (type0 == StmtType::Wildcard) {
      auto writer = make_unique<CFGBoolResultWriter<T>>(cfg, closure, result);
      writer->setParams(NO_STMT, arg1Val);
      return writer;
    }
    auto writer = make_unique<CFGSingleResultWriter<T, typePredicate>>(cfg, closure, result);
    writer->setParams(arg1Val, type0);
    return writer;
  }

  template<TypePredicate<T> typePredicate>
  ICFGWriterPtr makeRightWriter(StmtValue arg0Val, StmtType type1) const {
    if (type1 == StmtType::Wildcard) {
      auto writer = make_unique<CFGBoolResultWriter<T>>(cfg, closure, result);
      writer->setParams(arg0Val, NO_STMT);
      return writer;
    }
    auto writer = make_unique<CFGSingleResultWriter<T, typePredicate>>(cfg, closure, result);
    writer->setParams(arg0Val, type1);
    return writer;
  }

  template<TypePredicate<T> typePredicate>
  ICFGWriterPtr makePairWriter(StmtValue arg0Val, StmtType type0,
                               StmtType type1) {
    if (type0 == StmtType::Wildcard && type1 == StmtType::Wildcard) {
      auto writer = make_unique<CFGBoolResultWriter<T>>(cfg, closure, result);
      writer->setParams(arg0Val, NO_STMT);
      return writer;
    } else if (type0 != StmtType::Wildcard && type1 != StmtType::Wildcard) {
      auto writer = make_unique<CFGPairResultWriter<T, typePredicate>>(cfg, closure, result);
      writer->setParams(arg0Val, type1);
      return writer;
    } else if (type0 == StmtType::Wildcard) {
      auto writer = make_unique<CFGSingleResultWriter<T, typePredicate>>(cfg, closure, result);
      writer->setParams(arg0Val, type1);
      return writer;
    }
    auto writer = make_unique<CFGStaticLeftResultWriter<T>>(cfg, closure, result);
    writer->setLeft(arg0Val);
    return writer;
  }
};

template<class T>
CFGResultWriterFactory<T> makeCFGResultWriterFactory(CFG *cfg,
                                                     const T *closure,
                                                     StmtTransitiveResult *result) {
  return CFGResultWriterFactory(cfg, closure, result);
}
