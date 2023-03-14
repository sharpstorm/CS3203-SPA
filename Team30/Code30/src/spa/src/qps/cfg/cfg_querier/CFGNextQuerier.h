#pragma once

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"
#include "CFGQuerier.h"
#include "qps/cfg/CFGQuerierTypes.h"

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
class CFGNextQuerier: public ICFGClauseQuerier,
                      public CFGQuerier<
                          CFGNextQuerier<ClosureType, typePredicate>>{
 public:
  explicit CFGNextQuerier(CFG* cfg, ClosureType* closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) final;
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1) final;
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1) final;
  void queryAll(StmtTransitiveResult* resultOut,
                const StmtType &type0,
                const StmtType &type1) final;

 private:
  CFG* cfg;
  ClosureType* closure;
};

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
CFGNextQuerier<ClosureType, typePredicate>::CFGNextQuerier(
    CFG *cfg, ClosureType* closure): cfg(cfg), closure(closure) {}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)
      || !cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGNode nodeTo = cfg->toCFGNode(arg1);

  CFGLinks* links = cfg->nextLinksOf(nodeFrom);
  if (links == nullptr) {
    return result;
  }

  for (auto it = links->begin(); it != links->end(); it++) {
    if (*it == nodeTo) {
      result.add(arg0, arg1);
      break;
    }
  }

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  CFGNode nodeFrom = cfg->toCFGNode(arg0);
  CFGLinks* links = cfg->nextLinksOf(nodeFrom);
  for (auto it = links->begin(); it != links->end(); it++) {
    CFGNode node = *it;
    if (node == CFG_END_NODE) {
      continue;
    }

    int stmtNumber = cfg->fromCFGNode(node);
    if (!typePredicate(closure, type1, stmtNumber)) {
      continue;
    }
    result.add(arg0, stmtNumber);
  }

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextQuerier<ClosureType, typePredicate>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeTo = cfg->toCFGNode(arg1);
  CFGLinks* links = cfg->reverseLinksOf(nodeTo);
  for (auto it = links->begin(); it != links->end(); it++) {
    CFGNode node = *it;
    if (node == CFG_END_NODE) {
      continue;
    }

    int stmtNumber = cfg->fromCFGNode(node);
    if (!typePredicate(closure, type0, stmtNumber)) {
      continue;
    }
    result.add(cfg->fromCFGNode(node), stmtNumber);
  }

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
void CFGNextQuerier<ClosureType, typePredicate>::
queryAll(StmtTransitiveResult* resultOut,
         const StmtType &type0,
         const StmtType &type1) {
  for (CFGNode nodeFrom = 0; nodeFrom < cfg->getNodeCount(); nodeFrom++) {
    int fromStmtNumber = cfg->fromCFGNode(nodeFrom);
    if (!typePredicate(closure, type0, fromStmtNumber)) {
      continue;
    }

    CFGLinks* links = cfg->nextLinksOf(nodeFrom);
    for (auto it = links->begin(); it != links->end(); it++) {
      CFGNode nodeTo = *it;
      if (nodeTo == CFG_END_NODE) {
        continue;
      }

      int toStmtNumber = cfg->fromCFGNode(nodeTo);
      if (!typePredicate(closure, type1, toStmtNumber)) {
        continue;
      }
      resultOut->add(fromStmtNumber, toStmtNumber);
    }
  }
}
