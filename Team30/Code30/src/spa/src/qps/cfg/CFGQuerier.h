#pragma once

#include <memory>

#include "common/Types.h"
#include "common/cfg/CFG.h"
#include "CFGWalker.h"
#include "qps/cfg/cfg_querier/ICFGClauseQuerier.h"

#include "qps/cfg/cfg_querier/CFGNextQuerier.h"
#include "qps/cfg/cfg_querier/CFGNextTQuerier.h"

#include "CFGQuerierTypes.h"

using std::make_unique;

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
class CFGQuerier {
 public:
  explicit CFGQuerier(CFG* cfg, ClosureType* closure);
  StmtTransitiveResult queryNext(const StmtRef &arg0, const StmtRef &arg1);
  StmtTransitiveResult queryNextT(const StmtRef &arg0, const StmtRef &arg1);

 private:
  CFGWalker walker;
  ICFGQuerierPtr nextQuerier;
  ICFGQuerierPtr nextTQuerier;
  ClosureType* closure;

  StmtTransitiveResult queryArgs(ICFGClauseQuerier* caller,
                                 const StmtRef &arg0,
                                 const StmtRef &arg1);
};

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
CFGQuerier<ClosureType, typePredicate>::
CFGQuerier(CFG* cfg, ClosureType* closure):
    walker(cfg),
    closure(closure),
    nextQuerier(make_unique<
        CFGNextQuerier<ClosureType, typePredicate>>(cfg, closure)),
    nextTQuerier(make_unique<CFGNextTQuerier<ClosureType, typePredicate>>
                     (cfg, &walker, closure)) {}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGQuerier<ClosureType, typePredicate>::
queryNext(const StmtRef &arg0, const StmtRef &arg1) {
  return queryArgs(nextQuerier.get(), arg0, arg1);
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGQuerier<ClosureType, typePredicate>::
queryNextT(const StmtRef &arg0, const StmtRef &arg1) {
  return queryArgs(nextTQuerier.get(), arg0, arg1);
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGQuerier<ClosureType, typePredicate>::
queryArgs(ICFGClauseQuerier* caller, const StmtRef &arg0, const StmtRef &arg1) {
  if (arg0.isKnown() && arg1.isKnown()) {
    return caller->queryBool(arg0.lineNum, arg1.lineNum);
  } else if (arg0.isKnown()) {
    return caller->queryFrom(arg0.lineNum, arg1.type);
  } else if (arg1.isKnown()) {
    return caller->queryTo(arg0.type, arg1.lineNum);
  }
  return caller->queryAll(arg0.type, arg1.type);
}
