#pragma once

#include <vector>

#include "common/cfg/CFG.h"
#include "common/data_structs/BitField.h"
#include "qps/cfg/cfg_querier/CFGHaltWalkerException.h"

using std::vector;

template<typename T>
using WalkerSingleCallback = bool (*)(T *ptr, CFGNode node);

template<typename T>
using WalkerPairCallback = bool (*)(T *ptr,
                                    CFGNode nodeLeft,
                                    CFGNode nodeRight);

class CFGWalker {
  template <typename T>
  using DFSCallback = bool (*)(T *ptr, CFGNode node);
  using DFSLinkGetter = CFGLinks *(*)(CFG *cfg, CFGNode node);

 private:
  CFG *cfg;

 public:
  explicit CFGWalker(CFG *cfg);

  bool walkStatic(CFGNode start, CFGNode end);

  template<typename T, WalkerSingleCallback<T> callback>
  void walkFrom(CFGNode start, T *cbState);

  template<typename T, WalkerSingleCallback<T> callback>
  void walkTo(CFGNode end, T *cbState);

  template<typename T, WalkerPairCallback<T> callback>
  void walkAll(T *cbState);

 private:
  template<
      typename T, DFSCallback<T> callback,
      DFSLinkGetter stepGetter>
  void runDFS(CFGNode start, T *state);

  template<
      typename T, DFSCallback<T> callback,
      DFSLinkGetter stepGetter>
  void runDFSOn(CFGNode start, T *state);

  template<typename T, DFSCallback<T> callback>
  void runForwardDFS(CFGNode start, T *state);

  template<typename T, DFSCallback<T> callback>
  void runBackwardDFS(CFGNode start, T *state);

  static CFGLinks *forwardLinkGetter(CFG *cfg, CFGNode node);
  static CFGLinks *backwardLinkGetter(CFG *cfg, CFGNode node);

  template<typename T>
  struct NodewiseWalkerState {
    T *callbackState;
    WalkerSingleCallback<T> callback;
    CFG *cfg;
  };

  template<typename T>
  static constexpr bool nodewiseWalkerCallback(NodewiseWalkerState<T> *state,
                                     CFGNode node) {
    return state->callback(state->callbackState, node);
  }

  template<typename T>
  struct PairwiseWalkerState {
    T *cbState;
    WalkerPairCallback<T> callback;
    CFGNode startNode;
    CFG *cfg;
  };

  template<typename T>
  static constexpr bool pairwiseWalkerCallback(PairwiseWalkerState<T> *state,
                                        CFGNode node) {
    return state->callback(state->cbState, state->startNode, node);
  }
};

template<typename T, WalkerSingleCallback<T> callback>
void CFGWalker::walkFrom(CFGNode start, T *cbState) {
  NodewiseWalkerState<T> state{cbState, callback, cfg};
  runForwardDFS<NodewiseWalkerState<T>, nodewiseWalkerCallback<T>>(
      start, &state);
}

template<typename T, WalkerSingleCallback<T> callback>
void CFGWalker::walkTo(CFGNode end, T *cbState) {
  NodewiseWalkerState<T> state{cbState, callback, cfg};
  runBackwardDFS<NodewiseWalkerState<T>, nodewiseWalkerCallback<T>>(
      end, &state);
}

template<typename T, WalkerPairCallback<T> callback>
void CFGWalker::walkAll(T *cbState) {
  if (cfg->getNodeCount() == 0) {
    return;
  }

  for (CFGNode start = 0; start < cfg->getNodeCount(); start++) {
    PairwiseWalkerState<T> state{cbState, callback, start, cfg};
    runForwardDFS<PairwiseWalkerState<T>, pairwiseWalkerCallback<T>>(
        start, &state);
  }
}

template<typename T,
    CFGWalker::DFSCallback<T> callback,
    CFGWalker::DFSLinkGetter stepGetter>
void CFGWalker::runDFS(CFGNode start, T *state) {
  if (!cfg->containsNode(start)) {
    return;
  }

  try {
    runDFSOn<T, callback, stepGetter>(start, state);
  } catch (CFGHaltWalkerException) {
    // Expected halt, ignore signal
  }
}

template<typename T,
    CFGWalker::DFSCallback<T> callback,
    CFGWalker::DFSLinkGetter stepGetter>
void CFGWalker::runDFSOn(CFGNode start, T *state) {
  BitField seenNodes(cfg->getNodeCount());
  vector<CFGNode> currentNodes;

  currentNodes.push_back(start);
  while (!currentNodes.empty()) {
    CFGNode curNode = currentNodes.back();
    currentNodes.pop_back();

    auto step = stepGetter(cfg, curNode);
    for (auto it = step->begin(); it != step->end(); it++) {
      CFGNode nextNode = *it;
      if (nextNode == CFG_END_NODE) {
        continue;
      }

      if (seenNodes.isSet(nextNode)) {
        continue;
      }

      seenNodes.set(nextNode);
      bool shouldQueue = callback(state, nextNode);
      if (shouldQueue && nextNode != start) {
        currentNodes.push_back(nextNode);
      }
    }
  }
}

template<typename T, CFGWalker::DFSCallback<T> callback>
void CFGWalker::runForwardDFS(CFGNode start, T *state) {
  runDFS<T, callback, forwardLinkGetter>(start, state);
}

template<typename T, CFGWalker::DFSCallback<T> callback>
void CFGWalker::runBackwardDFS(CFGNode start, T *state) {
  runDFS<T, callback, backwardLinkGetter>(start, state);
}
