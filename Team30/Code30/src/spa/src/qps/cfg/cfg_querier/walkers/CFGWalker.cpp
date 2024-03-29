#include "CFGWalker.h"

#include "CFGHaltWalkerException.h"


CFGWalker::CFGWalker(CFG *cfg) : cfg(cfg) {
}

struct StaticWalkerState {
  bool isFound;
  CFGNode target;
};

constexpr bool staticWalkerCallback(StaticWalkerState *state, CFGNode node) {
  if (node == state->target) {
    state->isFound = true;
    throw CFGHaltWalkerException();
  }
  return true;
}

bool CFGWalker::walkStatic(CFGNode start, CFGNode end) {
  StaticWalkerState state{false, end};
  runForwardDFS<StaticWalkerState, staticWalkerCallback>(start, &state);

  return state.isFound;
}

CFGLinks *CFGWalker::forwardLinkGetter(CFG *cfg, CFGNode node) {
  return cfg->nextLinksOf(node);
}

CFGLinks *CFGWalker::backwardLinkGetter(CFG *cfg, CFGNode node) {
  return cfg->reverseLinksOf(node);
}
