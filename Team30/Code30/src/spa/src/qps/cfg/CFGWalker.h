#pragma once

#include <queue>

#include "common/cfg/CFG.h"
#include "common/BitField.h"

using std::queue;

template <typename T>
using WalkerSingleCallback = void(*)(T* ptr, int stmtNo);

template <typename T>
using WalkerPairCallback = void(*)(T* ptr, int stmtNo);

template <typename T>
using BFSCallback = bool(*)(T* ptr, CFGNode node);

template <typename T>
using BFSLinkGetter = T*(*)(CFG* cfg, CFGNode node);

class CFGWalker {
 private:
  BitField seenNodes;
  CFG* cfg;

 public:
  explicit CFGWalker(CFG* cfg);

  bool walkStatic(CFG* cfg, CFGNode start, CFGNode end);

  template <typename T, WalkerSingleCallback<T> callback>
  void walkFrom(CFG* cfg, CFGNode start);

  template <typename T, WalkerSingleCallback<T> callback>
  void walkTo(CFG* cfg, CFGNode end);

  template <typename T, WalkerPairCallback<T> callback>
  void walkAll(CFG* cfg);

 private:
  template <
      typename T, BFSCallback<T> callback,
      typename U, BFSLinkGetter<U> stepGetter>
  void runBFS(CFGNode start, T* state) {
    if (!cfg->containsNode(start)) {
      return;
    }

    queue<CFGNode> currentNodes;
    seenNodes.set(start);
    auto firstSteps = stepGetter(cfg, start);
    for (auto it = firstSteps->begin(); it != firstSteps->end(); it++) {
      currentNodes.push(*it);
    }

    while (!currentNodes.empty()) {
      CFGNode curNode = currentNodes.front();
      currentNodes.pop();

      auto step = stepGetter(cfg, curNode);
      for (auto it = step->begin(); it != step->end(); it++) {
        CFGNode nextNode = *it;
        bool shouldContinue = callback(state, nextNode);
        if (!shouldContinue) {
          return;
        }

        if (!seenNodes.isSet(nextNode)) {
          currentNodes.push(nextNode);
        }
      }
    }
  }

  static CFGForwardLink* forwardLinkGetter(CFG* cfg, CFGNode node) {
    return cfg->nextLinksOf(node);
  }

  template <typename T, BFSCallback<T> callback>
  void runForwardBFS(CFGNode start, T* state) {
    runBFS<T, callback, CFGForwardLink, forwardLinkGetter>(start, state);
  }

  static CFGBackwardLink* backwardLinkGetter(CFG* cfg, CFGNode node) {
    return cfg->reverseLinksOf(node);
  }

  template <typename T, BFSCallback<T> callback>
  void runBackwardBFS(CFGNode start, T* state) {
    runBFS<T, callback, CFGBackwardLink, backwardLinkGetter>(start, state);
  }
};
