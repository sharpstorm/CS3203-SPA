#pragma once

#include "common/Types.h"
#include "common/cfg/CFG.h"
#include "CFGWalker.h"
#include "qps/cfg/cfg_querier/ICFGClauseQuerier.h"

class CFGQuerier {
 public:
  explicit CFGQuerier(CFG* cfg);
  StmtTransitiveResult queryNext(const StmtRef &arg0, const StmtRef &arg1);
  StmtTransitiveResult queryNextT(const StmtRef &arg0, const StmtRef &arg1);

 private:
  CFGWalker walker;
  ICFGQuerierPtr nextQuerier;
  ICFGQuerierPtr nextTQuerier;

  StmtTransitiveResult queryArgs(ICFGClauseQuerier* caller,
                                 const StmtRef &arg0,
                                 const StmtRef &arg1);
};
