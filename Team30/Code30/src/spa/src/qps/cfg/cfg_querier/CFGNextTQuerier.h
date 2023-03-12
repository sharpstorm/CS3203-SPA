#pragma once

#include "ICFGClauseQuerier.h"
#include "common/cfg/CFG.h"
#include "qps/cfg/CFGWalker.h"

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
class CFGNextTQuerier: public ICFGClauseQuerier {
 public:
  CFGNextTQuerier(CFG *cfg, CFGWalker* walker, ClosureType* closure);

  StmtTransitiveResult queryBool(const StmtValue &arg0,
                                 const StmtValue &arg1) override;
  StmtTransitiveResult queryFrom(const StmtValue &arg0,
                                 const StmtType &type1) override;
  StmtTransitiveResult queryTo(const StmtType &type0,
                               const StmtValue &arg1) override;
  StmtTransitiveResult queryAll(const StmtType &type0,
                                const StmtType &type1) override;

 private:
  CFG* cfg;
  CFGWalker* walker;
  ClosureType* closure;

  struct ResultClosure {
    CFG* cfg;
    ClosureType* closure;
    StmtTransitiveResult* result;
    StmtType arg0Type;
    StmtType arg1Type;
  };
};

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
CFGNextTQuerier<ClosureType, typePredicate>::
CFGNextTQuerier(CFG *cfg, CFGWalker *walker, ClosureType* closure):
    cfg(cfg), walker(walker), closure(closure) {}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextTQuerier<ClosureType, typePredicate>::
queryBool(const StmtValue &arg0, const StmtValue &arg1) {
  StmtTransitiveResult result;

  if (!cfg->containsStatement(arg0)
      || !cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeStart = cfg->toCFGNode(arg0);
  CFGNode nodeEnd = cfg->toCFGNode(arg1);

  if (walker->walkStatic(nodeStart, nodeEnd)) {
    result.add(nodeStart, nodeEnd);
  }

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextTQuerier<ClosureType, typePredicate>::
queryFrom(const StmtValue &arg0, const StmtType &type1) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, closure, &result, StmtType::None, type1 };

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  constexpr WalkerSingleCallback<ResultClosure> callback =
      [](ResultClosure *state, CFGNode node) {
        int stmtNumber = state->cfg->fromCFGNode(node);
        if (!typePredicate(state->closure, state->arg1Type, stmtNumber)) {
          return;
        }
        state->result->add(0, stmtNumber);
      };

  CFGNode nodeStart = cfg->toCFGNode(arg0);
  walker->walkFrom<ResultClosure, callback>(nodeStart, &state);

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextTQuerier<ClosureType, typePredicate>::
queryTo(const StmtType &type0, const StmtValue &arg1) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, closure, &result, type0, StmtType::None };

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  constexpr WalkerSingleCallback<ResultClosure> callback =
      [](ResultClosure *state, CFGNode node) {
        int stmtNumber = state->cfg->fromCFGNode(node);
        if (!typePredicate(state->closure, state->arg0Type, stmtNumber)) {
          return;
        }
        state->result->add(stmtNumber, 0);
      };

  CFGNode nodeEnd = cfg->toCFGNode(arg1);
  walker->walkTo<ResultClosure, callback>(nodeEnd, &state);

  return result;
}

template <class ClosureType, StmtTypePredicate<ClosureType> typePredicate>
StmtTransitiveResult CFGNextTQuerier<ClosureType, typePredicate>::
queryAll(const StmtType &type0, const StmtType &type1) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, closure, &result, type0, type1 };

  constexpr WalkerPairCallback<ResultClosure> callback =
      [](ResultClosure *state, CFGNode nodeLeft, CFGNode nodeRight) {
        int fromStmtNumber = state->cfg->fromCFGNode(nodeLeft);
        int toStmtNumber = state->cfg->fromCFGNode(nodeRight);
        if (!typePredicate(state->closure, state->arg0Type, fromStmtNumber)
            || !typePredicate(state->closure, state->arg1Type, toStmtNumber)) {
          return;
        }
        state->result->add(fromStmtNumber, toStmtNumber);
      };
  walker->walkAll<ResultClosure, callback>(&state);

  return result;
}
