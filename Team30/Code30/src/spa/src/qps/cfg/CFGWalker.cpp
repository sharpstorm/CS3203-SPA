#include "CFGWalker.h"

#include <set>
#include <utility>

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
    return false;
  }
  return true;
}

bool CFGWalker::walkStatic(CFGNode start, CFGNode end) {
  StaticWalkerState state{false, end};
  runForwardDFS<StaticWalkerState, staticWalkerCallback>(start, &state);

  return state.isFound;
}
