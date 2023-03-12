#include "CFGNextTQuerier.h"

CFGNextTQuerier::CFGNextTQuerier(CFG *cfg, CFGWalker *walker):
    cfg(cfg), walker(walker) {}

StmtTransitiveResult CFGNextTQuerier::queryBool(const StmtValue &arg0,
                                                const StmtValue &arg1) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, &result };

  if (!cfg->containsStatement(arg0) || !cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeStart = cfg->toCFGNode(arg0);
  CFGNode nodeEnd = cfg->toCFGNode(arg1);

  if (walker->walkStatic(nodeStart, nodeEnd)) {
    result.add(nodeStart, nodeEnd);
  }

  return result;
}

StmtTransitiveResult CFGNextTQuerier::queryFrom(const StmtValue &arg0) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, &result };

  if (!cfg->containsStatement(arg0)) {
    return result;
  }

  CFGNode nodeStart = cfg->toCFGNode(arg0);
  walker->walkFrom<ResultClosure, nodeWalkFromCallback>(nodeStart,
                                                        &state);

  return result;
}

StmtTransitiveResult CFGNextTQuerier::queryTo(const StmtValue &arg1) {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, &result };

  if (!cfg->containsStatement(arg1)) {
    return result;
  }

  CFGNode nodeEnd = cfg->toCFGNode(arg1);
  walker->walkTo<ResultClosure, nodeWalkToCallback>(nodeEnd,
                                                    &state);

  return result;
}

StmtTransitiveResult CFGNextTQuerier::queryAll() {
  StmtTransitiveResult result;
  ResultClosure state{ cfg, &result };
  walker->walkAll<ResultClosure, pairWalkCallback>(&state);

  return result;
}

void CFGNextTQuerier::nodeWalkFromCallback(ResultClosure *state,
                                           CFGNode node) {
  state->result->add(0, state->cfg->fromCFGNode(node));
}

void CFGNextTQuerier::nodeWalkToCallback(ResultClosure *state,
                                         CFGNode node) {
  state->result->add(state->cfg->fromCFGNode(node), 0);
}

void CFGNextTQuerier::pairWalkCallback(ResultClosure *state,
                                       CFGNode nodeLeft,
                                       CFGNode nodeRight) {
  state->result->add(state->cfg->fromCFGNode(nodeLeft),
                     state->cfg->fromCFGNode(nodeRight));
}
