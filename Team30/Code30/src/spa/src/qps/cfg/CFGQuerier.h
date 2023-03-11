#pragma once

#include "common/Types.h"
#include "common/cfg/CFG.h"

typedef QueryResult<StmtValue, StmtValue> StmtTransitiveResult;

class CFGQuerier {
 public:
  explicit CFGQuerier(CFG* cfg);
  StmtTransitiveResult queryNext(const StmtRef &arg0, const StmtRef &arg1);

 private:
  CFG* cfg;

  StmtTransitiveResult queryBool(const StmtValue &arg0, const StmtValue &arg1);
  StmtTransitiveResult queryFrom(const StmtValue &arg0);
  StmtTransitiveResult queryTo(const StmtValue &arg1);
  StmtTransitiveResult queryAll();
};
