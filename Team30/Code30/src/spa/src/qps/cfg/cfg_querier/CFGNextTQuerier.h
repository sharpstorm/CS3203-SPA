#pragma once

#include "ICFGQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"

class CFGNextTQuerier: public ICFGQuerier {
 public:
  CFGNextTQuerier(CFG *cfg, CFGWalker* walker);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) override;
  StmtTransitiveResult queryFrom(const StmtValue &arg0) override;
  StmtTransitiveResult queryTo(const StmtValue &arg1) override;
  StmtTransitiveResult queryAll() override;

 private:
  CFG* cfg;
  CFGWalker* walker;

  struct ResultClosure {
    CFG* cfg;
    StmtTransitiveResult* result;
  };

  static void nodeWalkFromCallback(ResultClosure* state,
                                   CFGNode node);
  static void nodeWalkToCallback(ResultClosure* state,
                                 CFGNode node);
  static void pairWalkCallback(ResultClosure* state,
                               CFGNode nodeLeft,
                               CFGNode nodeRight);
};
