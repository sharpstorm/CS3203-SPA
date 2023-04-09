#pragma once

#include <vector>

#include "common/cfg/CFG.h"
#include "common/data_structs/BitField.h"
#include "qps/cfg/cfg_querier/CFGHaltWalkerException.h"
#include "ParallelStack.h"

using std::vector;

template<typename T>
using StatefulWalkerSingleCallback = BitField (*)(T *ptr, CFGNode node,
                                              BitField currentState);

template<class T, StatefulWalkerSingleCallback<T> callback>
class CFGStatefulWalker {
  using DFSLinkGetter = CFGLinks *(*)(CFG *cfg, CFGNode node);

 private:
  CFG *cfg;
  T *state;

 public:
  explicit CFGStatefulWalker(CFG *cfg, T *state) : cfg(cfg), state(state) {}

  void walkTo(CFGNode end, const BitField &initialState);

 private:
  template<DFSLinkGetter stepGetter>
  void runDFS(CFGNode start, const BitField &initialState);

  template<DFSLinkGetter stepGetter>
  void runDFSOn(CFGNode start, const BitField &initialState);

  template<DFSLinkGetter stepGetter>
  void exploreNode(vector<BitField> *visited,
                   ParallelStack<CFGNode, BitField> *stack);

  static CFGLinks *backwardLinkGetter(CFG *cfg, CFGNode node) {
    return cfg->reverseLinksOf(node);
  }

  void runBackwardDFS(CFGNode start, const BitField initialState) {
    runDFS<backwardLinkGetter>(start, initialState);
  }
};

template<typename T, StatefulWalkerSingleCallback<T> callback>
template<typename CFGStatefulWalker<T, callback>::DFSLinkGetter stepGetter>
void CFGStatefulWalker<T, callback>::runDFS(CFGNode start,
                                            const BitField &initialState) {
  if (!cfg->containsNode(start)) {
    return;
  }

  try {
    runDFSOn<stepGetter>(start, initialState);
  } catch (CFGHaltWalkerException) {
    // Expected halt, ignore signal
  }
}

template<typename T, StatefulWalkerSingleCallback<T> callback>
template<typename CFGStatefulWalker<T, callback>::DFSLinkGetter stepGetter>
void CFGStatefulWalker<T, callback>::runDFSOn(CFGNode start,
                                              const BitField &initialState) {
  vector<BitField> visitedNodes(cfg->getNodeCount());

  ParallelStack<CFGNode, BitField> currentNodes;
  currentNodes.push(start, initialState);
  while (!currentNodes.empty()) {
    exploreNode<stepGetter>(&visitedNodes, &currentNodes);
  }
}

template<typename T, StatefulWalkerSingleCallback<T> callback>
template<typename CFGStatefulWalker<T, callback>::DFSLinkGetter stepGetter>
void CFGStatefulWalker<T, callback>::
exploreNode(vector<BitField> *visitedNodes,
            ParallelStack<CFGNode, BitField> *currentNodes) {
  CFGNode curNode = currentNodes->getLeftBack();
  BitField curState = currentNodes->getRightBack();
  currentNodes->pop();

  auto step = stepGetter(cfg, curNode);
  for (const CFGNode nextNode : *step) {
    if (nextNode == CFG_END_NODE) {
      continue;
    }

    BitField newState = callback(state, nextNode, curState);
    auto const curHistory = visitedNodes->at(nextNode);
    if (curHistory.contains(newState)) {
      continue;
    }
    if (!newState.empty()) {
      BitField difference = curHistory.projectOnto(newState);
      currentNodes->push(nextNode, difference);
    }
    (*visitedNodes)[nextNode] = curHistory.unionWith(newState);
  }
}

template<typename T, StatefulWalkerSingleCallback<T> callback>
void CFGStatefulWalker<T, callback>::walkTo(CFGNode end,
                                            const BitField &initialState) {
  runBackwardDFS(end, initialState);
}
