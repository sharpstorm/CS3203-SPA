#pragma once

#include <vector>
#include <queue>

#include "common/cfg/CFG.h"
#include "common/data_structs/BitField.h"
#include "qps/cfg/cfg_querier/CFGHaltWalkerException.h"

using std::vector, std::queue;

template <typename T>
using StatefulWalkerSingleCallback = BitField(*)(T* ptr, CFGNode node,
                                                 BitField currentState);

template <typename T>
using StatefulDFSCallback = StatefulWalkerSingleCallback<T>;

using DFSLinkGetter = CFGLinks*(*)(CFG* cfg, CFGNode node);

class CFGStatefulWalker {
 private:
  CFG* cfg;

 public:
  explicit CFGStatefulWalker(CFG *cfg): cfg(cfg) {
  }

  template <typename T, StatefulWalkerSingleCallback<T> callback>
  void walkTo(CFGNode end, BitField initialState, T* cbState);

  template <typename T, StatefulWalkerSingleCallback<T> callback>
  void walkFrom(CFGNode end, BitField initialState, T* cbState);

 private:
  template <typename T, StatefulDFSCallback<T> callback,
      DFSLinkGetter stepGetter>
  void runDFS(CFGNode start, const BitField &initialState, T* state) {
    if (!cfg->containsNode(start)) {
      return;
    }

    try {
      runDFSOn<T, callback, stepGetter>(start, initialState, state);
    } catch (CFGHaltWalkerException) {
      // Expected halt, ignore signal
    }
  }

  template <typename T, StatefulDFSCallback<T> callback,
      DFSLinkGetter stepGetter>
  void runDFSOn(CFGNode start, const BitField &initialState, T* closure) {
    vector<BitField> visitedNodes(cfg->getNodeCount());

    vector<CFGNode> currentNodes;
    vector<BitField> pathStates;

    currentNodes.push_back(start);
    pathStates.push_back(initialState);

    while (!currentNodes.empty()) {
      CFGNode curNode = currentNodes.back();
      BitField curState = pathStates.back();
      currentNodes.pop_back();
      pathStates.pop_back();

      auto step = stepGetter(cfg, curNode);
      for (auto it = step->begin(); it != step->end(); it++) {
        CFGNode nextNode = *it;
        if (nextNode == CFG_END_NODE) {
          continue;
        }

        BitField newState = callback(closure, nextNode, curState);
        BitField difference = visitedNodes[nextNode].projectOnto(newState);
        if (visitedNodes[nextNode].contains(newState)) {
          continue;
        }
        visitedNodes[nextNode] = visitedNodes[nextNode].unionWith(newState);
        if (!newState.empty()) {
          currentNodes.push_back(nextNode);
          pathStates.push_back(difference);
        }
      }
    }
  }

  static CFGLinks* forwardLinkGetter(CFG* cfg, CFGNode node) {
    return cfg->nextLinksOf(node);
  }

  template <typename T, StatefulDFSCallback<T> callback>
  void runForwardDFS(CFGNode start, BitField initialState, T* state) {
    runDFS<T, callback, forwardLinkGetter>(start, initialState, state);
  }


  static CFGLinks* backwardLinkGetter(CFG* cfg, CFGNode node) {
    return cfg->reverseLinksOf(node);
  }

  template <typename T, StatefulDFSCallback<T> callback>
  void runBackwardDFS(CFGNode start, BitField initialState, T* state) {
    runDFS<T, callback, backwardLinkGetter>(start, initialState, state);
  }
};

template <typename T>
struct StatefulNodewiseWalkerState {
  T* callbackState;
  StatefulWalkerSingleCallback<T> callback;
  CFG* cfg;
};

template <typename T>
BitField statefulNodewiseWalkerCallback(
    StatefulNodewiseWalkerState<T>* state,
    CFGNode node,
    BitField currentState) {
  return state->callback(state->callbackState, node, currentState);
}

template <typename T, StatefulWalkerSingleCallback<T> callback>
void CFGStatefulWalker::walkTo(CFGNode end,
                               BitField initialState,
                               T* cbState) {
  StatefulNodewiseWalkerState<T> state{ cbState, callback, cfg };
  runBackwardDFS<StatefulNodewiseWalkerState<T>,
                 statefulNodewiseWalkerCallback<T>>(
      end, initialState, &state);
}

template <typename T, StatefulWalkerSingleCallback<T> callback>
void CFGStatefulWalker::walkFrom(CFGNode end,
                                 BitField initialState,
                                 T* cbState) {
  StatefulNodewiseWalkerState<T> state{ cbState, callback, cfg };
  runForwardDFS<StatefulNodewiseWalkerState<T>,
                statefulNodewiseWalkerCallback<T>>(
      end, initialState, &state);
}
