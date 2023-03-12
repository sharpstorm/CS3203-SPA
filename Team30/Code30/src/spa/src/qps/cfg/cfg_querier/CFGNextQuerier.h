#pragma once

#include "ICFGQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"

class CFGNextQuerier: public ICFGQuerier {
 public:
  explicit CFGNextQuerier(CFG *cfg);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) override;
  StmtTransitiveResult queryFrom(const StmtValue &arg0) override;
  StmtTransitiveResult queryTo(const StmtValue &arg1) override;
  StmtTransitiveResult queryAll() override;

 private:
  CFG* cfg;
};
