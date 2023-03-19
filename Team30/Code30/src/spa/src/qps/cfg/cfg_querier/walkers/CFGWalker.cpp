#include "CFGWalker.h"

#include <set>
#include <utility>

#include "qps/cfg/cfg_querier/CFGHaltWalkerException.h"


using std::set, std::pair;

CFGWalker::CFGWalker(CFG *cfg): cfg(cfg) {
}

struct StaticWalkerState {
  bool isFound;
  CFGNode target;
};

constexpr bool staticWalkerCallback(StaticWalkerState* state, CFGNode node) {
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