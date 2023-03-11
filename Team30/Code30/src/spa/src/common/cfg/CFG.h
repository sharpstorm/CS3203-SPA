#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "common/UtilityTypes.h"
#include "CFGLinks.h"

using std::string, std::vector, std::shared_ptr;

// CFG can only handle up to 65534 Nodes
class CFG {
 public:
  CFG(const string &name, const int &start);
  CFG();

  void addLink(const int &from, const int &to);
  bool contains(const CFGNode &node);
  CFGNode getStartingNode();

  CFGForwardLink* nextLinksOf(CFGNode node);
  CFGBackwardLink* reverseLinksOf(CFGNode node);

 private:
  string procedureName;
  int startingLineIndex;

  vector<CFGForwardLink> forwardLinks;
  vector<CFGBackwardLink> backwardLinks;
  CFGBackwardLink endNodeBackwardLink;

  void increaseMapSize(int num);

  template <class T>
  constexpr void addLink(T* target, const int &from, const int &to) {
    assert(from != CFG_NO_NODE);

    if (from == CFG_END_NODE) {
      return;
    }
    int fromIndex = from - startingLineIndex;
    int toIndex = (to == CFG_END_NODE) ?
                  CFG_END_NODE : to - startingLineIndex;

    if (target->size() < fromIndex + 1) {
      increaseMapSize(fromIndex + 1);
    }

    target->at(fromIndex).addLink(toIndex);
  }
};

typedef shared_ptr<CFG> CFGSPtr;
