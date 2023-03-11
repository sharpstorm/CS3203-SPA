#include "CFGWalker.h"

#include <set>

using std::set;

CFGWalker::CFGWalker(CFG *cfg): cfg(cfg), seenNodes(cfg->getNodeCount()) {
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
};

bool CFGWalker::walkStatic(CFG *cfg, CFGNode start, CFGNode end) {
  StaticWalkerState state{false, end};
  runForwardBFS<StaticWalkerState, staticWalkerCallback>(start, &state);

  return state.isFound;
}

template <typename T>
struct NodewiseWalkerState {
  WalkerSingleCallback<T> callback;
  CFG* cfg;
};

template <typename T>
constexpr bool nodewiseWalkerCallback(NodewiseWalkerState<T>* state,
                                      CFGNode node) {
  state->callback(state->cfg->fromCFGNode(node));
  return true;
};

template <typename T, WalkerSingleCallback<T> callback>
void CFGWalker::walkFrom(CFG *cfg, CFGNode start) {
  NodewiseWalkerState<T> state{callback, cfg};
  runForwardBFS<NodewiseWalkerState<T>, nodewiseWalkerCallback<T>>(
      start, &state);
}

template <typename T, WalkerSingleCallback<T> callback>
void CFGWalker::walkTo(CFG *cfg, CFGNode end) {
  NodewiseWalkerState<T> state{callback, cfg};
  runBackwardBFS<NodewiseWalkerState<T>, nodewiseWalkerCallback<T>>(
      end, &state);
}

template <typename T>
struct PairwiseWalkerState {
  WalkerPairCallback<T> callback;
  CFG* cfg;
  queue<CFGNode> seenNodes;
  set<uint32_t> seenPairs;
};

template <typename T>
constexpr bool pairwiseWalkerCallback(NodewiseWalkerState<T>* state,
                                      CFGNode node) {
  state->callback(state->cfg->fromCFGNode(node));
  return true;
};

template<typename T, WalkerPairCallback<T> callback>
void CFGWalker::walkAll(CFG *cfg) {

}
