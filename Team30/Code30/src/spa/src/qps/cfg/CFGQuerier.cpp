#include "CFGQuerier.h"

#include <memory>

#include "qps/cfg/cfg_querier/CFGNextQuerier.h"
#include "qps/cfg/cfg_querier/CFGNextTQuerier.h"

using std::make_unique;

CFGQuerier::CFGQuerier(CFG *cfg):
    walker(cfg),
    nextQuerier(make_unique<CFGNextQuerier>(cfg)),
    nextTQuerier(make_unique<CFGNextTQuerier>(cfg, &walker)) {}

StmtTransitiveResult CFGQuerier::queryNext(const StmtRef &arg0,
                                           const StmtRef &arg1) {
  return queryArgs(nextQuerier.get(), arg0, arg1);
}

StmtTransitiveResult CFGQuerier::queryNextT(const StmtRef &arg0,
                                           const StmtRef &arg1) {
  return queryArgs(nextTQuerier.get(), arg0, arg1);
}

StmtTransitiveResult CFGQuerier::queryArgs(ICFGClauseQuerier* caller,
                                           const StmtRef &arg0,
                                           const StmtRef &arg1) {
  if (arg0.isKnown() && arg1.isKnown()) {
    return caller->queryBool(arg0.lineNum, arg1.lineNum);
  } else if (arg0.isKnown()) {
    return caller->queryFrom(arg0.lineNum);
  } else if (arg1.isKnown()) {
    return caller->queryTo(arg1.lineNum);
  }
  return caller->queryAll();
}
