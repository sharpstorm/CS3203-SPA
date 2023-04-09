#pragma once

#include "common/cfg/CFG.h"
#include "qps/cfg/cfg_querier/walkers/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/writers/ICFGResultWriter.h"
#include "qps/cfg/cfg_querier/writers/CFGResultWriterFactory.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
class CFGNextQuerier : public CFGQuerier<
    CFGNextQuerier<ClosureType, typePredicate>> {
 public:
  explicit CFGNextQuerier(CFG *cfg, const ClosureType &closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1);
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1);
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1);
  void queryAll(StmtTransitiveResult *resultOut,
                const StmtType &type0,
                const StmtType &type1);

 private:
  CFG *cfg;
  const ClosureType &closure;
};

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
CFGNextQuerier<ClosureType, typePredicate>::CFGNextQuerier(
    CFG *cfg, const ClosureType &closure): cfg(cfg), closure(closure) {}

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)
      || !cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGNode nodeTo = cfg->toCFGNode(arg1);

  CFGLinks *links = cfg->nextLinksOf(nodeFrom);
  if (links == nullptr) {
    return result;
  }

  for (const auto &node : *links) {
    if (node == nodeTo) {
      result.setNotEmpty();
      break;
    }
  }

  return result;
}

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGLinks *links = cfg->nextLinksOf(nodeFrom);
  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &closure, &result)
      .makeRightWriter<typePredicate>(arg0, type1);

  for (const CFGNode &node : *links) {
    if (node == CFG_END_NODE) {
      continue;
    }

    const StmtValue stmtNumber = writer->toStmtNumber(node);
    if (!writer->writeRight(stmtNumber)) {
      break;
    }
  }
  return result;
}

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  CFGLinks *links = cfg->reverseLinksOf(nodeTo);
  ICFGWriterPtr writer = makeCFGResultWriterFactory(cfg, &closure, &result)
      .makeLeftWriter<typePredicate>(type0, arg1);

  for (const CFGNode &node : *links) {
    if (node == CFG_END_NODE) {
      continue;
    }

    const StmtValue stmtNumber = writer->toStmtNumber(node);
    if (!writer->writeLeft(stmtNumber)) {
      break;
    }
  }

  return result;
}

template<class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
void CFGNextQuerier<ClosureType, typePredicate>::
queryAll(StmtTransitiveResult *resultOut, const StmtType &type0,
         const StmtType &type1) {
  auto factory = makeCFGResultWriterFactory(cfg, &closure, resultOut);
  ICFGWriterPtr writer = factory.makePairWriter<typePredicate>(0, type0, type1);

  for (CFGNode nodeFrom = 0; nodeFrom < cfg->getNodeCount(); nodeFrom++) {
    StmtValue fromStmtNumber = cfg->fromCFGNode(nodeFrom);
    writer->setLeft(fromStmtNumber);
    if (!typePredicate(closure, type0, fromStmtNumber)) {
      continue;
    }

    CFGLinks *links = cfg->nextLinksOf(nodeFrom);
    for (const CFGNode &nodeTo : *links) {
      if (nodeTo == CFG_END_NODE) {
        continue;
      }

      StmtValue toStmtNumber = cfg->fromCFGNode(nodeTo);
      if (!writer->writeRight(toStmtNumber)) {
        break;
      }
    }
  }
}
